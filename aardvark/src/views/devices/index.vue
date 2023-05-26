<template>
  <div>
    <div class="fill-height fill-width overflow-hidden">
      <template>
        <v-btn class="mr-2" depressed tile @click="testGetMenu">
          test get menu
        </v-btn>

        <v-btn class="mr-2" depressed tile @click="testGetInfo">
          test get info
        </v-btn>
      </template>
      <p>info = {{ info }}</p>
      <p>devices {{ devices }}</p>
    </div>
    div class="fill-height fill-width overflow-hidden">
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
      <template #search>
        <v-row class="px-4">
          <v-col class="py-0" cols="12">
            <v-text-field v-model="query.name" autofocus clearable placeholder="Please enter a keyword query" />
          </v-col>
        </v-row>
      </template>

      <template #actions>
        <v-btn class="mr-2" depressed tile @click="handleAdd">
          Add item
        </v-btn>
      </template>

      <template #[`item.number`]="{ index }">
        {{ index + 1 }}
      </template>

      <template #[`item.time`]="{ item }">
        <v-chip :color="item.time >= 60 ? 'primary' : 'dark'">
          {{ item.time }}
        </v-chip>
      </template>

      <template #[`item.occupy`]="{ item }">
        {{ item.occupy ? 'Yes' : 'No' }}
      </template>

      <template #[`item.actions`]="{ item }">
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleConnect(item.id)" v-on="on">
              rowing
            </v-icon>
          </template>
          <span>connect</span>
        </v-tooltip>


        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleEdit(item.id)" v-on="on">
              edit
            </v-icon>
          </template>
          <span>edit</span>
        </v-tooltip>


        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon color="red" v-bind="attrs" @click="handleDelete(item.id)" v-on="on">
              delete
            </v-icon>
          </template>
          <span>delete</span>
        </v-tooltip>
      </template>
    </DataTable>

    <DeviceSchema
      ref="DeviceSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>
import DeviceSchema from './modules/DeviceSchema.vue'
import {deleteDevice, getDeviceList} from '@/api/device'
import toast from '@/utils/toast'
import {getInfo, getMenus} from '@/api/esp'
import {EspMutations} from "@/store/modules";
import store from "@/store";


export default {
  name: 'DeviceList',
  components: {
    DeviceSchema,
  },
  props: {
    id: {
      type: String,
      default: 'Vue!',
    },
  },
  data: () => ({
    query: {
      name: '',
    },
  }),
  computed: {


    headers () {
      return [
        {
          text: 'Number',
          align: 'center',
          sortable: false,
          value: 'number',
          width: 100,
          fixed: true,
        },
        {
          text: 'Device index',
          align: 'center',
          sortable: false,
          value: 'name',
        },
        {
          text: 'Speed (reading per min)',
          align: 'center',
          value: 'time',
          width: 100,
        },
        {
          text: 'item category',
          align: 'center',
          sortable: false,
          value: 'category',
          width: 120,
        },
        {
          text: 'Size',
          align: 'center',
          value: 'price',
          width: 120,
        },
        {
          text: 'item type',
          align: 'center',
          sortable: false,
          value: 'type',
          width: 120,
        },
        {
          text: 'GitHub Source',
          align: 'center',
          value: 'occupy',
          width: 100,
        },
        {
          text: 'Rating (%)',
          align: 'center',
          value: 'percent',
          width: 100,
        },
        {
          text: 'Update time',
          align: 'center',
          value: 'lastModifyTime',
          width: 150,
        },
        {
          text: 'Operation',
          align: 'center',
          sortable: false,
          value: 'actions',
          width: 110,
          fixed: true,
        },
      ]
    },
  },
  mounted () {
    console.log("ere")
  },
  methods: {

    /**
     * Added items
     * @return {Undefined}
     */
    testGetMenu () {
      this.loadGetMenu().then(a => alert(a))
    },


    async loadGetMenu () {
      await getMenus(null)
    },

    testGetInfo () {

      store.dispatch(`esp/${EspMutations.ESP_FUNCTION}`, ["cat"])
      this.loadGetInfo().then(a => alert(a))
    },

    async loadGetInfo () {
      await getInfo(null)
    }, /**
     * Call the interface data and initialize the table
     * @return {Promise<Undefined>}
     */
    async loadData (options = {}) {
      return getDeviceList({...this.query, ...options}).then(r => r.data)
    },
    /**
     * Added items
     * @return {Undefined}
     */
    handleAdd () {
      this.$refs['DeviceSchema'].open()
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
    handleEdit (id) {
      this.$refs['DeviceSchema'].open(id)
    },
    /**
     * connect item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleConnect (id) {
      // this call into device
      this.$refs['DeviceSchema'].getItem(id)

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

    store.dispatch(`esp/${EspMutations.ESP_FUNCTION}`, ["cat"])
    this.loadGetInfo().then(a => alert(a))
  },

  async loadGetInfo () {
    await getInfo(null)
  },

}
</script>

<style lang="scss">
</style>
