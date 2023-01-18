<template>
  <v-app id="app" class="fill-width fill-height">
    <v-fade-transition leave-absolute mode="out-in">
      <router-view />
    </v-fade-transition>
    <Toast :dark="$store.state.setting.appThemeDark" />
  </v-app>
</template>

<script>
import cookies from "vue-cookies";

export default {
  name: 'App',
  created () {
    // this manually set the devices
    const j = {devices: ['esp-home']}
    console.log(JSON.stringify(j))
    cookies.set("ANT", JSON.stringify(j))
  },
}
</script>

<style lang="scss">
@import '~@/assets/styles/index';
</style>
