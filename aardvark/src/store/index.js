import Vue from 'vue'
import Vuex from 'vuex'
import VuexPersistence from 'vuex-persist'
import modules from './modules'

Vue.use(Vuex)

const persistence = new VuexPersistence({
  key: 'Aardvark',
  storage: window.localStorage,
  reducer: ({
    account,
    devices,
    setting: {appPermanentNavigation, appPrimaryColor, appThemeDark, appMultipleTabs, appShowHidden},
  }) => {
    return ({
      account,
      devices,
      setting: {appPermanentNavigation, appPrimaryColor, appThemeDark, appMultipleTabs, appShowHidden},
    });
  },
})

export default new Vuex.Store({
  plugins: [
    persistence.plugin,
  ],
  modules,
})
