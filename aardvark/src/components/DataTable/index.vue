<template>
  <div class="data-table fill-width fill-height d-flex flex-column">
    <CssStyle :content="fixedColumnsStyle" />

    <v-form>
      <slot name="search" />
      <slot name="headerButtons" />
    </v-form>

    <div :style="{ position: 'relative' }" class="flex-grow-1 overflow-hidden">
      <v-data-table
        ref="table"
        :footer-props="{
          itemsPerPageText: 'Number of items per page',
          itemsPerPageOptions: [4,7,10,15, 20, 30, 50],
          showCurrentPage: true,
          showFirstLastPage: true,
        }"
        :headers="headers"
        :hide-default-footer="false"
        :item-key="itemKey"
        :items="items"
        :multi-sort="multiSort"
        :no-data-text="loading ? 'Loading...' : 'No data'"
        :options="options"
        :server-items-length="total || 0"
        class="elevation-0 fill-width fill-height d-flex flex-column overflow-x-hidden"
        fixed-header
        locale="zh-cn"
        @update:options="fetch($event)"
      >
        <template v-for="(_, slot) of $scopedSlots" #[slot]="scope">
          <slot :name="slot" v-bind="scope" />
        </template>
      </v-data-table>
<!--      <VLoading :value="loading" absolute />-->
    </div>
  </div>
</template>

<script>
import VLoading from '@/components/VImplements/VLoading.vue'
import CssStyle from '@/components/CssStyle/index.vue'

export default {
  name: 'DataTable',
  components: {
    VLoading,
    CssStyle,
  },
  props: {
    loadData: {
      type: Function,
      default: () => Promise.resolve(),
      required: true,
    },
    headers: {
      type: Array,
      default: () => [],
      required: true,
    },
    itemKey: {
      type: String,
      default: 'id',
      required: true,
    },
    multiSort: {
      type: Boolean,
      default: false,
    },
    defaultOptions: {
      type: Object,
      default: () => ({}),
    },
  },
  data () {
    return {
      items: [],
      loading: false,
      options: Object.assign({
        itemsPerPage: 20,
        page: 1,
        sortBy: [],
        sortDesc: [],
      }, this.defaultOptions),
      $tableWrapper: null,
      total: 0,
    }
  },
  computed: {
    fixedColumnsStyle () {
      const {left = [], right = []} = this.pickFixedColumns()
      return [
        ...this.calcFixedColumnCls(left, true),
        ...this.calcFixedColumnCls(right, false),
      ].join('\r\n')
    },
  },
  methods: {
    async fetch (payload = {}) {
      try {
        this.loading = true
        const {items, total} = await this.loadData(Object.assign(this.options, payload))

        Object.assign(this, {items, total})
        await this.$nextTick()
        await this.scrollToTop()
      } catch (e) {
        this.items = []
        this.total = 0
        throw e
      } finally {
        this.loading = false
      }
    },
    refresh (firstPage = false) {
      if (firstPage && this.options.page !== 1) {
        this.options.page = 1
      } else {
        this.fetch(this.options)
      }
    },
    scrollToTop () {
      this.$tableWrapper = this.$tableWrapper || this.$refs['table'].$el.getElementsByClassName('v-data-table__wrapper')[0]
      return this.$vuetify.goTo(0, {
        container: this.$tableWrapper,
      })
    },
    pickFixedColumns () {
      if ([0, 1].includes(this.headers.length)) {
        return {}
      }

      const [left] = this.headers
      const [right] = this.headers.slice(-1)
      return {
        left: left.fixed ? [1] : [],
        right: right.fixed ? [1] : [],
      }
    },
    // TODO: calc multiple sticky items' left / right
    calcFixedColumnCls (cols = [], left = false) {
      const rootSelector = '.data-table:not(.v-data-table--mobile)'
      const nth = num => `nth${left ? '' : '-last'}-child(${num})`

      return cols.map(col => `
        ${rootSelector} tbody tr > td:${nth(col)},
        ${rootSelector} thead tr > th:${nth(col)} {
          background: var(--background-color);
          position: sticky;
          ${left ? 'left' : 'right'}: 0;
          z-index: 2;
        }

        ${rootSelector} tbody tr:hover > td:${nth(col)} {
          background: inherit;
        }

        ${rootSelector} thead > tr > th:${nth(col)} {
          z-index: 3;
        }
      `)
    },
  },
}
</script>

<style lang="scss">
.theme--dark .data-table {
  --background-color: #1e1e1e;
}

.data-table {
  --background-color: #fff;
  position: static !important;

  .v-pagination {
    text-align: right !important;
    width: auto !important;
  }

  .v-toolbar__content {
    padding-bottom: 0;
    padding-top: 0;
  }

  .v-data-footer {
    font-size: 14px;
  }

  th,
  td {
    /* stylelint-disable-next-line */
    @extend .text-no-wrap !optional;
  }
}
</style>
