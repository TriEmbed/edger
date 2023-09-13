<template>
  <div class="fill-height fill-width overflow-hidden">
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

      <template #headerButtons>
        <div class="d-flex flex-row pb-1 px-2">
          <v-btn class="mr-2" depressed tile @click="handleAdd">
            Add item
          </v-btn>
          <v-spacer />
          <v-btn class="mr-2" depressed tile type="submit" @click.stop.prevent="refresh(true)">
            Inquire
          </v-btn>
          <v-btn depressed tile @click="refresh()">
            refresh
          </v-btn>
        </div>
      </template>

      <template #[`item.actions`]="{ item }">
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

    <ProjectSchema
      ref="projectSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>
import ProjectSchema from './modules/ProjectSchema.vue'
import {deleteProject, getProjectList} from '@/api/project'
import toast from '@/utils/toast'

export default {
  name: 'ProjectList',
  components: {
    ProjectSchema,
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
          text: 'Project index',
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
  methods: {
    /**
     * Call the interface data and initialize the table
     * @return {Promise<Undefined>}
     */
    async loadData (options = {}) {
      return getProjectList({...this.query, ...options}).then(r => r.data)
    },
    /**
     * Added items
     * @return {Undefined}
     */
    handleAdd () {
      this.$refs['projectSchema'].open()
    },
    /**
     * Added project successfully
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
      this.$refs['projectSchema'].open(id)
    },
    /**
     * Edit project success
     * @return {Undefined}
     */
    handleEditSuccess () {
      toast.success({message: 'Editing project successful'})
      this.$refs['table'].refresh()
    },
    /**
     * delete item
     * @param {Number | String} id item id
     * @return {Promise<Undefined>}
     */
    async handleDelete (id) {
      await deleteProject(id)
      toast.success({message: 'Delete the item successfully'})
      await this.$refs.table.refresh()
    },
  },
}
</script>

<style lang="scss">
</style>
