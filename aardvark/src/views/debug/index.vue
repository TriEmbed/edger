<template>
  <div class="fill-height fill-width overflow-hidden">
    <template>
      <v-btn class="mr-2" depressed tile @click="testGetMenu">
        test get menu
      </v-btn>

      <v-btn class="mr-2" depressed tile @click="testGetInfo">
        test get info
      </v-btn>
    </template>
    <p>{{ info }}</p>
    <p>devices {{ devices }}</p>
  </div>
</template>

<script>
import {getInfo, getMenus} from '@/api/esp'
import {EspMutations} from "@/store/modules";
import store from "@/store";


export default {
  name: 'HomeList',
  components: {},

  computed: {
    info () {
      return store.state.esp.openedRoutes
    },
    devices () {
      return store.state.devices
    },
  },
  methods: {
    /**

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
    },
  },
}
</script>

<style lang="scss">
</style>
