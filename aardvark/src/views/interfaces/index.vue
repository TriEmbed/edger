<template>
  <div>
    <div class="d-flex justify-space-around">
      <v-menu activator="#gpio-menu-activator">
        <v-list>
          <v-list-item
            v-for="(item, index) in gpioMenuItems"
            :key="index"
            :value="index"
            @click="handleGPIOMenuClick(index)"
          >
            <v-list-item-title>{{ item.title }}</v-list-item-title>
          </v-list-item>
        </v-list>
      </v-menu>

      <v-menu activator="#interface-menu-activator">
        <v-list>
          <v-list-item
            v-for="(item, index) in interfaceMenuItems"
            :key="index"
            :value="index"
            @click="handleInterfaceMenuClick(index)"
          >
            <v-list-item-title>{{ item.title }}</v-list-item-title>
          </v-list-item>
        </v-list>
      </v-menu>
      <v-menu activator="#i2c-menu-activator">
        <v-list>
          <v-list-item
            v-for="(item, index) in i2cMenuItems"
            :key="index"
            :value="index"
            @click="handleI2CMenuClick(index)"
          >
            <v-list-item-title>{{ item.title }}</v-list-item-title>
          </v-list-item>
        </v-list>
      </v-menu>
    </div>
    <Scan id="viewScanner" />

    <DataTable
      id="topTemplate"
      ref="table"

      :default-options="{
        sortBy: ['lastModifyTime'],
        sortDesc: [true],
      }"
      :headers="headers"
      :load-data="loadData"
      item-key="id"
    >
      <template #[`item.number`]="{ index }">
        {{ index + 1 }}
      </template>

      <template #headerButtons>
        <v-tabs>
          <v-tab
            v-for="tabs of InterfaceItems"
            :id="tabs.id"
            :key="tabs.tab"
            :title="tabs.tab"
            @click="HandleInterfaceSelect(tabs.tab)"
          >
            {{ tabs.tab }}
          </v-tab>
        </v-tabs>


        <div class="d-flex flex-row pb-1 px-2">
          <v-spacer />
          <v-btn class="mr-2" depressed tile @click="handleAdd(tag)">
            Add item
          </v-btn>

          <v-btn class="mr-2" depressed tile @click="handleAdd">
            Add item
          </v-btn>
        </div>
      </template>


      <template #[`item.actions`]="{ item }">
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="testGetMenu(item.IP)" v-on="on">
              rowing
            </v-icon>
          </template>
          <span>connect</span>
        </v-tooltip>
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleEdit(item)" v-on="on">
              edit
            </v-icon>
          </template>
          <span>information</span>
        </v-tooltip>
      </template>
    </DataTable>
    <vue-grid-sheet
      id="gridTemplate"
      ref="grid"
      :data="demoBasic.data"
      :format="demoBasic.format"
      :state="demoBasic.state"
    />
    <InterfaceSchema
      ref="InterfaceSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
    <GPIOSchema
      ref="GPIOSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
    <I2CSchema
      ref="I2CSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>


import {deleteDevice} from '@/api/device'
import toast from '@/utils/toast'

import store from "@/store";
import InterfaceSchema from "@/views/interfaces/modules/InterfaceSchema.vue";
import GPIOSchema from '@/views/interfaces/modules/GPIOSchema.vue'
import I2CSchema from '@/views/interfaces/modules/I2CSchema.vue'
import VueGridSheet from "@/components/gridsheet/gridsheet.vue";
import Scan from '@/views/esp/scani2c.vue'

const example = [...Array(20).keys()]
  .map(i => `A${i} B${i} C${i} D${i} D${i}`)
  .join("\n");
const makeData = () => {
  let val = []
  // for each colum
  for (let colIndex = 0; colIndex < 16; colIndex++) {
    // the corner element id empty
    // fill the top row
    let rows = colIndex < 8 ? [colIndex.toString(16)] : [""]
    for (let rowIndex = 0; rowIndex < 16; rowIndex++) {
      rows.push(colIndex < 8 ? (colIndex * 16 + rowIndex).toString(16) : "");
    }
    val.push(rows)
  }
  //val.splice(8,8)
  return val;
};
export default {
  name: 'Interface',

  props: {
    id: {
      type: Number,
      default: 0,
    },
  },
  components: {
    Scan,
    VueGridSheet,
    I2CSchema,
    InterfaceSchema: InterfaceSchema,
    GPIOSchema: GPIOSchema,
  },
  data: () => ({
    query: {
      name: 'index 1',
    },
    width: 70,
    demoBasic: {
      format: [{noedit: true}],
      state: [[]],
      data: makeData(),
    },
    dataSrc: example,
    mode: "basic",
    edit: false,


    interfaceMenuItems: [
      {title: 'Scan'},
      {title: 'set i2c'},
    ],

    gpioMenuItems: [
      {title: 'set'},
      {title: 'read'},
      {title: 'write'},
    ],
    i2cMenuItems: [
      {title: 'read'},
      {title: 'write'},
      {title: 'dialog'},
    ],

    InterfaceItem: InterfaceSchema,
    InterfaceItems: [
      {tab: InterfaceSchema.tag, Schema: InterfaceSchema, id: "interface-menu-activator"},
      {tab: GPIOSchema.tag, Schema: GPIOSchema, id: "gpio-menu-activator"},
      {tab: I2CSchema.tag, Schema: I2CSchema, id: "i2c-menu-activator"},
      // {tab: 'gpio', schema: "GPIOSchema",header: gpioHeaders ,devices: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 2]},
      // {tab: 'i2c', schema: "I2CSchema", header: i2cHeaders, devices: ["dialog"]},
      {tab: 'spi', schema: "SPISchema"},
      {tab: 'serial', schema: "SERIALSchema"},
      {tab: 'wifi', schema: 'WIFISchema'},

    ],
    tab: InterfaceSchema.tag,// set to first InterfaceItems:
  }),

  computed: {

    headers () {
      const k = this.InterfaceItem
      console.log(k)
      return k.header

    },
  },
  mounted () {
    const data = this.$store.state.ants.antList
    const index = parseInt(this.$route.params['id'])
    console.log(`the component is now mounted.`, index)
    console.log(`call by`, JSON.stringify(data[index]))
    document.getElementById("viewScanner").style.display = 'none'
    const parentID = document.getElementById("topTemplate")
    const tableID = parentID.getElementsByClassName('flex-grow-1')[0]
    const scanID = document.getElementById("gridTemplate")

    scanID.style.visibility = 'visible';
    scanID.style.display = ""
    tableID.style.visibility = 'hidden';
    tableID.style.display = 'none';
  },
  methods: {
    handleGPIOMenuClick: function (index) {
      console.log(' handleGPIOMenuClick: function(index)', index)
    },
    handleI2CMenuClick: function (index) {
      console.log(' handleI2CMenuClick: function(index)', index)
    },
    handleInterfaceMenuClick: function (index) {
      console.log(' handleInterfaceMenuClick: function(index)', index)
    },
    setDisplay: function () {

      const parentID = document.getElementById("topTemplate")
      const tableID = parentID.getElementsByClassName('flex-grow-1')[0]
      const scanID = document.getElementById("gridTemplate")
      if (this.tab === "Interface") {
        scanID.style.visibility = 'visible';
        scanID.style.display = ""
        tableID.style.visibility = 'hidden';
        tableID.style.display = 'none';
      } else {
        tableID.style.display = ""
        tableID.style.visibility = 'visible';
        scanID.style.visibility = 'hidden';
        scanID.style.display = 'none';
      }


    },
    HandleInterfaceSelect: function (tab) {
      this.tab = tab

      this.InterfaceItem = this.InterfaceItems.filter(d => d.tab === tab)[0].Schema
      this.setDisplay()
      this.$refs['table'].refresh()
      console.info(tab)
    },
    /**
     * Added items
     * @return {Undefined}
     */
    testGetMenu (id) {
      this.loadGetMenu().then(a => alert(a))
    },


    async loadGetMenu () {

      await getMenus(null)
    },

    testGetInfo () {

      store.dispatch(`deviceinstance/${DeviceInstanceMutations.DEVICEINSTANCE_FUNCTION}`, ["cat"])
      this.loadGetInfo().then(a => alert(a))
    },

    async loadGetInfo () {
      await getInfo(null)
    },
    /**
     * Call the interface data and initialize the table
     * @return {Promise<Undefined>}
     */
    getItem: function () {
      const r = this.InterfaceItem.deviceinstance

      return r
    },
    loadData: async function (options = {}) {

      return new Promise((resolve) => {
        let r = []
        r = this.getItem()

        resolve({total: r.length, items: r})
      })

    },
    // async loadData (options = {}) {
    //   return getDeviceList({...this.query, ...options}).then(r => r.data)
    // },
    /**
     * Added items
     * @return {Undefined}
     */
    handleAdd () {
      let item = this.InterfaceItems.filter(d => d.tab === this.tab)[0]
      this.$refs[item.tab + 'Schema'].open({})
    },
    /**
     * Added device successfully
     * @return {Undefined}
     */
    handleAddSuccess () {
      toast.success({message: 'Add item successfully'})
      this.query = this.$options.data.apply(this).query
      this.$refs['table'].refresh(true)
    },
    /**
     * Edit item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleEdit (item) {
      console.log(item.tag + "Schema")
      //      let q = this.InterfaceItems.filter(d => d.tab === tab)[0].Schema
      this.$refs[item.tag + 'Schema'].open(item)
    },
    /**
     * connect item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleConnect (id) {
      // this call into device
      this.$refs['InterfaceSchema'].getItem(id)

    },
    /**
     * Edit device success
     * @return {Undefined}
     */
    handleEditSuccess () {
      toast.success({message: 'Editing device successful'})
      this.$refs['table'].refresh()
    },
    /**
     * delete item
     * @param {Number | String} id item id
     * @return {Promise<Undefined>}
     */
    async handleDelete (id) {
      await deleteDevice(id)
      toast.success({message: 'Delete the item successfully'})
      await this.$refs.table.refresh()
    },
  },


  /**
   * Added items
   * @return {Undefined}
   */
  testGetMenu () {
    this.loadGetMenu().then(
      a => {
        alert(a)
      },
    )
  },

  async loadGetMenu () {
    await getMenus(null)
  },

  testGetInfo () {

    store.dispatch(`deviceinstance/${DeviceInstanceMutations.DEVICEINSTANCE_FUNCTION}`, ["cat"])
    this.loadGetInfo().then(a => alert(a))
  },

  async loadGetInfo () {
    await getInfo(null)
  },

}
</script>

<style lang="scss">
</style>
