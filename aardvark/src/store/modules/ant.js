//import {AccountActions, AccountMutations} from "@/store/modules/account";

export const AntMutations = {
  ANTS: 'ANTS',
}

export default {
  namespaced: true,
  state: {
    antList: [],
    mDNSofCurrentAnt: '',
  },
  mutations: {

    [AntMutations.ANTS] (state, routes = []) {

      for (const id of state.antList) {
        if (id['MAC'] === routes['MAC']) {
          return
        }
      }
      routes["id"] = state.antList.length
      state.antList.push(routes)
    },
  },

  actions: {
    async [AntMutations.ANTS] ({commit}, payload) {
      commit(AntMutations.ANTS, payload)
    },
  },
}


