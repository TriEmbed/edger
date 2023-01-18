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


export let deviceLists = []
//
const openDevice = (name) => {
  const address = "http://" + name + ".local"
  return axios.create({
    baseURL: address, timeout: 4000,
    responseType: "json",
    mode: "no_cors",
    headers: {},
  });
};

const readMenu = (tmpList, url) => {
  tmpList['val'].get(url)
    .then((response) => {

      console.log(response.data)


      let newMenu = store.getters["account/getMenu"]

      let done = false
      newMenu.forEach(function (item) {
        console.log(item.text, tmpList["device"])
        if (item.text === tmpList["device"])
          done = true;

      })
      if (done) {
        return
      }
      const p = Object.assign({}, newMenu[0]);

      p.text = tmpList["device"]
      p.path = "/esp/i2c"
      p.name = "cat"
      p.type = 'MENU'
      p.to = "/device"
      p.children = response.data.menus
      newMenu.splice(1, 0, p)
      //       tmpList['result'] = response.data
      deviceLists.push(tmpList)

      store.dispatch(`account/${AccountActions.BUILD_ROUTES}`).then(
        console.log("routes"),
      )
    })
    .catch((err) => console.log((err)))
};

export const set = function (address,index,data) {

  const dev = cookies.get("ANT").devices[0]
  const files = openDevice(dev)

  const v = {address: address , index: index};

  const k= JSON.stringify(v)
  files.patch("/api/v1/i2c?set=" + encodeURI(k),{data: data })
    .then((response) => {
      console.log("done 1")
      store.dispatch(`esp/${EspMutations.ESP_I2C_SET}`, response.data).then(r => {
        console.log("done 2")
      })
      console.log(response.data)
    })
}


export const getInfo = function (data = {}) {
  const dev = cookies.get("ANT").devices[0]
  const files = openDevice(dev)

  files.get("/api/v1/system/info")
    .then((response) => {
      this.$store.dispatch(`esp/${EspMutations.ESP_INFO}`, response.data).then(r => {
        console.log(r)
      })
      console.log(response.data)
    })
}

//http://192.168.100.106/api/v1/i2c?scan=



const i2cHandler= function (result)
{
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



// despite the name this is the key interface, it takes params and will take the data
// constructs an interface to the ESP using the information passed it the menu item
// https://axios-http.com/docs/req_config
router.currentRoute.meta.path = undefined;
export const getPatch = function (params, data) {
  const r = router.currentRoute.meta.path
  const url = new URL(r)

  ///////////////////////////////////////////////
  // let see what we have got
  console.log(r)
  console.log(JSON.stringify(url))
  console.log("hostname",url.hostname)
  console.log('pathname',url.pathname)
  console.log('search',url.search)
  console.log('path',url.path)

  // build an axios which either succeed or does a catch
  axios({
    method: 'patch',
    params: params,
    url: "http://" + url.hostname + url.pathname,
    responseType: 'json', // default
  }).then((result) => { // success
    ///////////////////////////////////////////////
    // get the type
    let command=url['pathname'].split('/').pop()
    console.log(command)
    switch (command){
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

export const getMenus = function (data = {}) {

  const dev = cookies.get("ANT").devices
  // console.log(router.getRoutes())

  dev.forEach(function (deviceFromCookies) {
    let tmpList = {}

    tmpList['device'] = deviceFromCookies
    /**
     * Import Axios
     */
    tmpList['val'] = openDevice(tmpList['device'])

    if (tmpList['val']) {
      readMenu(tmpList, "/api/v1/system/menu")
    }
  })// loop through
}// getProjectList

