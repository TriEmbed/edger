<template>
  <div class="toast">
    <v-fade-transition group mode="out-in">
      <v-snackbar
        v-for="(item, i) in items"
        :key="item.id"
        :color="item.color"
        :dark="dark"
        :timeout="item.timeout"
        :top="true"
        :value="i === 0"
        absolute
        app
      >
        {{ item.message }}
        <template #action="{ attrs }">
          <v-btn
            v-if="items.length > 1"
            color="white"
            text
            v-bind="attrs"
            @click="dequeue(item.id)"
          >
            next （{{ items.length - 1 }} +）
          </v-btn>
          <v-btn
            v-else
            color="white"
            icon
            text
            v-bind="attrs"
            @click="dequeue(item.id)"
          >
            <v-icon>close</v-icon>
          </v-btn>
        </template>
      </v-snackbar>
    </v-fade-transition>
  </div>
</template>

<script>
import event, {EVENT_TYPE_ADD_ITEM} from './event'

export default {
  name: 'Toast',
  data: () => ({
    timeoutId: null,
    items: [],
  }),
  props: {
    dark: {
      type: Boolean,
      default: false,
    },
  },
  methods: {
    handleAddItem ({id = Date.now() + Math.random(), message, color, timeout}) {
      this.items.push({id, message, color, timeout})
      this.enqueue()
    },
    enqueue () {
      const [{id, timeout}] = this.items
      if (!this.timeoutId) {
        this.timeoutId = setTimeout(this.dequeue, timeout, id)
      }
    },
    dequeue (id) {
      if (this.timeoutId) {
        clearTimeout(this.timeoutId)
        this.timeoutId = null
      }

      const index = this.items.findIndex(i => i.id === id)
      this.items.splice(index, 1)

      if (this.items.length) {
        this.enqueue()
      }
    },
  },
  created () {
    event.addListener(EVENT_TYPE_ADD_ITEM, this.handleAddItem)
  },
  beforeDestroy () {
    event.removeAllListeners()
  },
}
</script>

<style lang="scss">
.toast {
  .v-snack {
    position: fixed;
    z-index: 9;
  }
}
</style>
