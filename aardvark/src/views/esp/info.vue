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
    <pre>{{ info }}</pre>
  </div>
</template>

<script>

import {getInfo} from "@/api/esp";
import store from "@/store";

const extractor = path => path + "oK";

export default {
  name: 'EspIndo',
  computed: {
    info () {
      return JSON.stringify(store.state.esp.info, null, 2)
    },
  },
  // render () {
  //
  //   const here = extractor(this.$route.path)
  //
  //   return (
  //     <div class="home">
  //       <p>ESP {here}</p>
  //       <p>ESP { store.state.esp.info}</p>
  //     </div>
  //   )
  // },
  activated: function () {
    getInfo({one: this.$route.path, two: extractor(this.$route.path)})
    console.log("info activated()")
  },
  deactivated: function () {
    console.log("info deactivated()")
  },

}
</script>
import { getInfo} from '@/api/device'
<style lang="scss">
.home {
  height: 100%;
  position: relative;
  width: 100%;
}
</style>
