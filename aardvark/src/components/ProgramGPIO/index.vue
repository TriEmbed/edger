<template>
  <div>
    <DataTable
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

      <template #[`item.actions`]="{ item }">
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleConnect(item)" v-on="on">
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

    <gpioSchema
      ref="antSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>


import {deleteDevice} from '@/api/device'
import toast from '@/utils/toast'
import gpioSchema, {gpioSchema as thisSchema} from "@/components/ProgramGPIO/modules/gpioSchema.vue"
import {getInfo, getMenus} from "@/api/esp";


let loop = -1
const loopMax = 2
const delayTime = 500;
let mdnsNamesIndex = 0
const scanMDNSAddress = () => {


};

export default {
  name: 'ProgramGPIO',
  components: {
    gpioSchema,
  },
  computed: {
    infoData: function () {
      const data = this.$store.state.ants.antList

      let dataLength = data.length

      if (dataLength === 0)
        return "no ants"
      dataLength = data.length - 1
      console.log("scan new data", JSON.stringify(data[dataLength]));
      this.$refs['table'].refresh(true)
      return data[dataLength]['mdnsName'];
    },
    headers () {
      return gpioSchema.header
    },
  },
  data: () => ({
    query: {
      name: '',
    },
  }),
  async loadGetInfo () {
    await getInfo(null)
  },
  async loadGetMenu () {
    //  await getMenus(null)
  },
  methods: {
    action (index) {
      this.items[index].action()
      console.log(index)
    },
    //
    // /**
    //  * Added items
    //  * @return {Undefined}
    //  */
    // testGetMenu (id) {
    //   this.loadGetMenu().then(a => alert(a))
    // },
    //
    //
    // async loadGetInfo () {
    //   await getInfo(null)
    // },

    loadData: async function (options = {}) {
      return new Promise((resolve) => {
        const data = gpioSchema.data().gpioPins
        resolve({total: data.length, items: data})
      })
    },
    // async loadData (options = {}) {
    //   return getDeviceList({...this.query, ...options}).then(r => r.data)
    // },
    /**
     * Added items
     * @return {Undefined}
     */
    reScan () {
      loop = 0;
      scanMDNSAddress()
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

      console.log("thisSchema")
      //      let q = this.HomeItems.filter(d => d.tab === tab)[0].Schema

      this.$refs['antSchema'].open(item)
    },
    /**
     * connect item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleConnect (item) {
      // this call into device
      const addressMDNS = this.$store.state.ants.antList[item.id]["mdnsName"]
      console.log(addressMDNS)
      getMenus(addressMDNS)


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

  setup: function () {


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
  }
  ,


}
</script>

<style lang="scss">
</style>
