import Vue from "vue";
import Vuex from "vuex";

Vue.use(Vuex);

const store = new Vuex.Store({
  state: {
    count: 0,
  }, mutations: {
    reset: (state) => (state.count = 0), update: (state, payload) => (state.count += payload),
  }, actions: {
    countAction ({commit}, payload) {
      return new Promise(resolve => {
        setTimeout(() => {
          commit("update", payload);
          resolve();
        }, 1000);
      });
    }, resetCount ({commit}) {
      return new Promise(resolve => {
        setTimeout(() => {
          commit("reset");
          resolve();
        }, 500);
      });
    },
  },
});

store.watch(state => {
  return {a: state.count};
}, watched => {
  console.log("Count is:", watched.a);
}, {});

export default store;
