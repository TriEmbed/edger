import cookies from "vue-cookies";
import store from "@/store";
import {AccountActions, EspMutations} from '@/store/modules'
import axios from "axios";
import router from "@/router";

/**
 * Added items
 * @param {Object} data
 * @return {Promise<any>}
 */

const endPoints = {
  menu: "/api/v1/system/menu", info: "/api/v1/system/info", i2cSet: "/api/v1/i2c?set=",
}


export let deviceLists = []
//
const openDevice = (name) => {
  const address = "http://" + name + ".local"
  return axios.create({
    baseURL: address, timeout: 1000, responseType: "json", mode: "no_cors", headers: {},
  });
};
//
// const readMenu = (tmpList, url) => {
//   tmpList['val'].get(url)
//     .then((response) => {
//
//       console.log(response.data)
//       let newMenu = store.getters["account/getMenu"]
//
//       let done = false
//       newMenu.forEach(function (item) {
//         console.log(item.text, tmpList["device"])
//         if (item.text === tmpList["device"]) done = true;
//
//       })
//       if (done) {
//         return
//       }
//       const p = Object.assign({}, newMenu[0]);
//
//       p.text = tmpList["device"]
//       p.path = "/esp/i2c"
//       p.index = "cat"
//       p.type = 'MENU'
//       p.to = "/device"
//       p.children = response.data.menus
//       newMenu.splice(1, 0, p)
//       //       tmpList['result'] = response.data
//       deviceLists.push(tmpList)
//
//       store.dispatch(`account/${AccountActions.BUILD_ROUTES}`).then(console.log("routes"))
//     })
//     .catch((err) => console.log((err)))
// };

export const set = function (address, index, data) {

  const dev = cookies.get("ANT").devices[0]
  const files = openDevice(dev)

  const v = {address: address, index: index};

  const k = JSON.stringify(v)
  files.patch(endPoints.i2cSet + encodeURI(k), {data: data})
    .then((response) => {
      console.log("done 1")
      store.dispatch(`esp/${EspMutations.ESP_I2C_SET}`, response.data).then(r => {
        console.log("done 2")
      })
      console.log(response.data)
    })
}

export const getInfo = function (data = {address: null, nextCallback: null}) {
  if (data.address === null) {
    data.address = cookies.get("ANT").devices[0]
  }
  const next = data.nextCallback
  const files = openDevice(data.address)

  files.get(endPoints.info)
    .then((response) => {
      response.data['mDNS'] = data.address
      store.dispatch(`esp/${EspMutations.ESP_INFO}`, response.data).then(r => {
        console.log(r)
        //     console.log(this.$store.state["esp/info"])
        //   //this.$store.state.network.devices.push(response.data)
        //   //console.log(this.$store.state.network.devices.getter)
      })
      //console.log(this.$store.state["esp/info"])
      // console.log(response.data)
    }).catch(function (error) {
      console.log("error", error)
    }).finally(function () {
      if (next !== null) {
        next()
      }
      console.log("finally", next)
    })


}

//http://192.168.100.106/api/v1/i2c?scan=


const i2cHandler = function (result) {
  const data = result["data"]['i2c'];
  // noinspection DuplicatedCode
  data.forEach(function (item, idx) {
    const key = Object.keys(item)[0]
    const val = Object.values(data[idx])
    switch (key) {
      case 'info':
        console.log("info found", JSON.stringify(val[0]))
        store
          .dispatch(`esp/${EspMutations.ESP_INFO}`, JSON.stringify(val[0]))
          .then(() => {
            console.log("info done");
          });
        break;
      case 'scan':
        console.log("scan found", JSON.stringify(val[0]))
        store
          .dispatch(`esp/${EspMutations.ESP_I2C_SCAN}`, val[0])
          .then(() => {
            console.log("scan done");
          });
        break;
      case "get":
      case "set":
        break;
      default:
    }
  })

}


// despite the index this is the key interface, it takes params and will take the data
// constructs an interface to the ESP using the information passed it the menu item
// https://axios-http.com/docs/req_config
router.currentRoute.meta.path = undefined;
export const getPatch = function (params, data) {
  const r = router.currentRoute.meta.path
  const url = new URL(r)
  // this.$store.state.ants.mDNSofCurrentAnt

  ///////////////////////////////////////////////
  // let see what we have got
  console.log(r)
  console.log(JSON.stringify(url))
  console.log("hostname", url.hostname)
  console.log('pathname', url.pathname)
  console.log('search', url.search)
  console.log('path', url.path)

  // build an axios which either succeed or does a catch
  axios({
    method: 'patch', params: params, url: "http://" + url.hostname + url.pathname, responseType: 'json', // default
  }).then((result) => { // success
    ///////////////////////////////////////////////
    // get the type
    let command = url['pathname'].split('/').pop()
    console.log(command)
    switch (command) {
      case "i2c":
        i2cHandler(result)
        break;
      default:
    }
  }).catch((error) => { // fail
    ///////////////////////////////////////////////
    console.log("error", error)
  })
};

// add a new set of menus
export const getMenus = function (ip = {}) {
  // if there no ip get address from cookies list necessary for older version
  if (ip.address === null) {
    ip = cookies.get("ANT").devices[0]
  }
  const files = openDevice(ip)

  files.get(endPoints.menu).then((response) => {

    const url = new URL(response.config.baseURL)
    console.log(url.hostname);
    let menuToSpliceIn = store.getters["account/getMenu"]
    let dup = false;
    // check to make sure we have not previously added the item
    menuToSpliceIn.forEach(function (existingMenuItem) {
      console.log(existingMenuItem.text)
      if (existingMenuItem.text === url.hostname) {
        dup = true
      }
    })
    if (dup) {
      return
    }
    const newTopItem = Object.assign({}, menuToSpliceIn);
    //
    newTopItem.text = url.hostname
    newTopItem.path = "/esp/i2c"
    newTopItem.name = "cat"
    newTopItem.type = 'MENU'
    newTopItem.temporary=true
    newTopItem.to = "/device"
    newTopItem.children = response.data.menus
    menuToSpliceIn.splice(1, 0, newTopItem)

    // add to existing menu list
    store.dispatch(`account/${AccountActions.BUILD_ROUTES}`).then(console.log("routes"))
  }).catch(function (error) {
    // handle error
    console.log("Error is: " + error);
  })

// loop through
}// getProjectList

