//import {AccountActions, AccountMutations} from "@/store/modules/account";

export const EspMutations = {
  ESP_FUNCTION: 'ESP_FUNCTION',
  ESP_INFO: 'ESP_INFO',
  ESP_I2C_SCAN: 'ESP_SCAN',
  ESP_I2C_SET: 'ESP_SET',

}

export default {
  namespaced: true,
  state: {
    openedRoutes: ["was"],
    info: "",
    scan: [],
    count: 0,

  },
  mutations: {
    increment: state => ++state.count,
    [EspMutations.ESP_FUNCTION] (state, routes = []) {
      state.openedRoutes = routes
    },
    [EspMutations.ESP_INFO] (state, routes = []) {
      state.info = routes
    },
    [EspMutations.ESP_I2C_SCAN] (state, routes = []) {
      state.scan = routes
    },
    [EspMutations.ESP_I2C_SET] (state, routes = []) {
      state.scan = routes
    },
  },

  actions: {

    async [EspMutations.ESP_FUNCTION] ({commit}, payload) {

      // const { data } = await login(payload)
      //  const { permissions, menus, token, username } = data
      //   commit(AccountMutations.SET_PERMISSIONS, permissions)
      commit(EspMutations.ESP_FUNCTION, payload)
      //   commit(AccountMutations.SET_TOKEN, token)
      //   commit(AccountMutations.SET_USER_NAME, username)
      //  await dispatch(AccountActions.BUILD_ROUTES)
    },
    async [EspMutations.ESP_INFO] ({commit}, payload) {

      // const { data } = await login(payload)
      //  const { permissions, menus, token, username } = data
      //   commit(AccountMutations.SET_PERMISSIONS, permissions)
      commit(EspMutations.ESP_INFO, payload)
      //   commit(AccountMutations.SET_TOKEN, token)
      //   commit(AccountMutations.SET_USER_NAME, username)
      //  await dispatch(AccountActions.BUILD_ROUTES)
    },
    async [EspMutations.ESP_I2C_SCAN] ({commit}, payload) {
      commit(EspMutations.ESP_I2C_SCAN, payload)
    },
    async [EspMutations.ESP_I2C_SET] ({commit}, payload) {
      commit(EspMutations.ESP_I2C_SET, payload)
    },
  },
}


