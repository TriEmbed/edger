<template>
  <v-navigation-drawer
    ref="drawer"
    :permanent="value"
    :value="value"
    :width="width"
    app
    class="FormWrapper"
    fixed
    flat
    right
    temporary
  >
    <!-- / Header -->
    <v-toolbar slot="prepend" :height="appHeaderHeight" color="primary darken-1" dark>
      <v-toolbar-title>
        {{ title }}
      </v-toolbar-title>
    </v-toolbar>

    <!-- / Content -->
    <v-card :flat="flat" class="overflow-y-auto" height="100%">
      <v-card-text class="flex-grow-1">
        <slot name="content" />
      </v-card-text>
    </v-card>

    <VLoading :value="loading" absolute />

    <!-- / Footer -->
    <template #append>
      <v-divider dark />
      <div class="fill-width d-flex flex-row">
        <slot name="actions" />
      </div>
    </template>
  </v-navigation-drawer>
</template>

<script>
import {mapState} from 'vuex'
import VLoading from '@/components/VImplements/VLoading.vue'

export default {
  name: 'FormDrawer',
  components: {
    VLoading,
  },
  props: {
    flat: {
      type: Boolean,
      default: false,
    },
    loading: {
      type: Boolean,
      default: false,
    },
    title: {
      type: String,
      default: '',
    },
    value: {
      type: Boolean,
      default: false,
    },
    width: {
      type: Number,
      default: 650,
    },
  },
  computed: {
    ...mapState('setting', ['appHeaderHeight']),
  },
}
</script>

<style lang="scss">
</style>
