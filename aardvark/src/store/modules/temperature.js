
//import {AccountActions, AccountMutations} from "@/store/modules/account";

import axios from "axios";

export const TemperatueMutations = {
  TEMPERATURES: 'TEMPERATURES',
  TEMPERATURES_READ: 'TEMPERATURES_READ',
}

export default {
  namespaced: true,
  state: {
    temperatureActiveList: [1, 2, 3, 4, 7],
  },
  mutations: {

    [TemperatueMutations.TEMPERATURES] (state, routes = []) {

      for (const id of state.temperatureActiveList) {
        if (id['MAC'] === routes['MAC']) {
          return
        }
      }
      routes["id"] = state.temperatureActiveList.length
      state.temperatureActiveList.push(routes)
    },
  },
  actions: {
    async [TemperatueMutations.TEMPERATURES_READ] ({commit}) {
      axios.get("http:/esp-home.local/api/v1/temp/raw")
        .then(data => {
          commit("temperatureActiveList", data.data.raw);
        })
        .catch(error => {
          console.log(error);
        });
    },
  },
}


// import Vue from 'vue'
// import Vuex from 'vuex'
// import axios from 'axios'
//
// Vue.use(Vuex)
// export const TempMutations = {
//   TEMPERATURE: 'TEMPERATURE',
//   TEMPERATURE_SCAN: 'TEMPERATURE_SCAN',
// }
// export default {
//   state: {
//     chart_value: [8, 2, 5, 9, 5, 11, 3, 5, 10, 0, 1, 8, 2, 9, 0, 13, 10, 7, 16],
//   },
//   mutations: {
//     [TempMutations.TEMPERATURE] (state, new_value) {
//       state.chart_value.push(new_value);
//       state.chart_value.shift();
//     },
//     [TempMutations.TEMPERATURE_SCAN] (state, routes = []) {
//       state.chart_value = routes
//     },
//   },
//   actions: {
//     async [TempMutations.TEMPERATURE_SCAN]({ commit }) {
//       axios.get("http:/esp-home.local/api/v1/temp/raw")
//         .then(data => {
//           commit("update_chart_value", data.data.raw);
//         })
//         .catch(error => {
//           console.log(error);
//         });
//     }
//   }
// }
