<template>
  <div class="exception fill-width d-flex flex-column justify-center align-center">
    <div :style="{ filter: appThemeDark ? 'invert(100%)': 'none' }" class="exception-img mb-6" />
    <h1>interface</h1>
    <h1>{{ config.title }}</h1>
    <h2>{{ config.desc }}</h2>
    <div class="mt-6">
      <v-btn class="primary mr-4" depressed type="primary" @click="$router.push('/home')">
        back to front page
      </v-btn>
      <v-btn class="primary" depressed type="primary" @click="logout">
        Log in again
      </v-btn>
    </div>
  </div>
</template>

<script>
import config from './config'
import {mapActions, mapGetters, mapState} from 'vuex'
import {AccountActions} from '@/store/modules'

export default {
  name: 'Exception',
  computed: {
    ...mapState('runtime', ['openedRoutes']),
    ...mapState('setting', ['appThemeDark']),
    ...mapGetters('account', ['hasLoggedIn']),
    config () {
      return config[this.$route.params.type]
    },
  },
  methods: {
    ...mapActions('account', {
      logout: AccountActions.LOGOUT,
    }),
  },
}
</script>

<style lang="scss">
.exception {
  height: 85%;

  &-img {
    background-image: url('~@/assets/images/alert-triangle.svg');
    background-position: center center;
    background-repeat: no-repeat;
    background-size: contain;
    height: 240px;
    width: 360px;
  }
}
</style>
