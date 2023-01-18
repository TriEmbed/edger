<template>
  <v-app-bar
    :clipped-left="appPermanentNavigation || $vuetify.breakpoint.lgAndUp"
    :height="appHeaderHeight"
    app
    class="pr-4"
    color="primary darken-2"
    dark
    flat
  >
    <v-toolbar-title class="ml-0 mr-1 pl-4 d-flex flex-row justify-center align-center" style="width: 50;">
      <v-expand-x-transition>
        <v-app-bar-nav-icon
          v-show="!appPermanentNavigation"
          @click.stop="toggleAppNavigation"
        />
      </v-expand-x-transition>
    </v-toolbar-title>
    <v-spacer />
    <span>{{ title }}</span>
    <v-spacer />
    <v-tooltip bottom>
      <template #activator="{ on }">
        <v-btn
          class="text-none"
          dark
          text
          @click="logout"
          v-on="on"
        >
          <span>{{ username }}</span>
        </v-btn>
      </template>
      <span>log out</span>
    </v-tooltip>

    <v-tooltip bottom>
      <template #activator="{ on }">
        <v-btn
          color="white"
          icon
          @click="toggleAppSetting"
          v-on="on"
        >
          <v-icon size="20">
            settings
          </v-icon>
        </v-btn>
      </template>
      <span>Personalize </span>
    </v-tooltip>

    <v-tooltip bottom>
      <template #activator="{ on }">
        <v-btn
          :href="repo"
          color="white"
          icon
          target="_blank"
          v-on="on"
        >
          <svg
            class="feather feather-github"
            fill="none"
            height="24"
            stroke="currentColor"
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-width="2"
            viewBox="0 0 24 24"
            width="24"
            xmlns="http://www.w3.org/2000/svg"
          >
            <path
              d="M9 19c-5 1.5-5-2.5-7-3m14 6v-3.87a3.37 3.37 0 0 0-.94-2.61c3.14-.35 6.44-1.54 6.44-7A5.44 5.44 0 0 0 20 4.77 5.07 5.07 0 0 0 19.91 1S18.73.65 16 2.48a13.38 13.38 0 0 0-7 0C6.27.65 5.09 1 5.09 1A5.07 5.07 0 0 0 5 4.77a5.44 5.44 0 0 0-1.5 3.78c0 5.42 3.3 6.61 6.44 7A3.37 3.37 0 0 0 9 18.13V22"
            />
          </svg>
        </v-btn>
      </template>
      <span>Github</span>
    </v-tooltip>
  </v-app-bar>
</template>

<script>
import mixin from './LayoutMixin.vue'

export default {
  name: 'AppHeader',
  mixins: [mixin],
  data: () => ({
    title: process.env.VUE_APP_TITLE,
    repo: process.env.VUE_APP_GITHUB_REPO,
  }),
}
</script>

<style lang="scss">

</style>
