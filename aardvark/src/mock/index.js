import Adaptor from 'axios-mock-adapter'
import request from '@/utils/request'
import _ from 'lodash-es'
import cookies from "vue-cookies";
import buildMenuItem from "@/utils/myRoutes"
import deviceLists from "@/api/esp"
import store from "@/store";

const packageAdaptor = new Adaptor(request, {delayResponse: 300})

let devices = []


// when we have logged in this build the menu
packageAdaptor.onPost(/api\/login/).reply(config => {
  const {username} = JSON.parse(config.data)


  const k = cookies.get("ANT").devices
  k.forEach(ant => {
    if (devices.indexOf(ant) === -1) {
      devices.push(ant);
    }
  })

  console.log((deviceLists))
  const val = [200, {
    username,
    token: 'ac21ebab-bddc-41a3-bef5-4ecf3325c888',
    permissions: [],
    menus: [

      buildMenuItem("home", "home", false, "/home", [], 'VIEW', 'home/index'),
      // buildMenuItem('interfaces', null, false, '/device/:id', [], 'VIEW', 'interfaces/index'),
      buildMenuItem('Configuration', 'apps', false, '/project', [], 'MENU', '',
        [
          //buildMenuItem('project list', null, false, '/project/list', [], 'VIEW', 'project/index'),
          buildMenuItem('device list', null, false, '/device/list', [], 'VIEW', 'device/index'),
        ]),
      buildMenuItem("Temperature", "home", false, "/esp/Temperature", [], 'VIEW', 'esp/Temperature'),
      // buildMenuItem('Chart', 'priority_high', false, '/Chart', [], 'VIEW', '', null, 'Chart'),
      buildMenuItem('404', 'priority_high', true, '404_test', [], 'VIEW', '', null, '/exception/404'),
    ],
  }];
  // console.info(devices)
  // devices.forEach((value, index) => {
  //   console.log("board", value, index)
  //   const k = buildMenuItem(value, 'apps', false, '/esp/index', [], 'MENU', null,
  //       [
  //         buildMenuItem('info', null, false, '/esp/api/system/info:id=' + value, [], 'VIEW', 'esp/info'),
  //         buildMenuItem('ls' +
  //             '', null, false, '/esp/api/system/i2c:id=' + value, [], 'VIEW', 'esp/i2c'),
  //       ])
  //
  //   val[1].menus.splice(1 + index, 0, k)
  // })
  val[1].menus[1]['props'] = true
  return val
})

const projectItem = (id = 1) => ({
  id,
  name: Math.random().toString(36).substring(7),
  time: (Math.random() * 30 + 40).toFixed(0),
  category: Math.random() > 0.5 ? 'MIT' : 'GNU',
  percent: (Math.random() * 30 + 40).toFixed(0),
  price: (Math.random() * 400 + 100).toFixed(0),
  occupy: Math.random() > 0.5,
  type: ['Dialog 70004', 'Analog Device', 'STM', 'interface', 'Data Collection'][`${Math.abs(Math.random() - 0.5)}`[2]],
  tags: ['dehumidification', 'blood circulation', 'sleep aid', 'ventilation', 'beauty'][`${Math.abs(Math.random() - 0.5)}`[2]],
  lastModifyTime: (function (d) {
    const Y = d.getFullYear()
    const M = d.getMonth() + 1
    const D = d.getDate()
    const H = d.getHours()
    const m = d.getMinutes()
    const s = d.getSeconds()
    const pad = num => num.toString().padStart(2, '0')
    return `${Y}-${pad(M)}-${pad(D)} ${pad(H)}:${pad(m)}:${pad(s)}`
  })(new Date()),
})

packageAdaptor.onPost(/\/api\/project/).reply(200)
packageAdaptor.onPut(/\/api\/project/).reply(200)
packageAdaptor.onDelete(/\/api\/project\/\d+/).reply(200)
packageAdaptor.onGet(/\/api\/project\/\d+/).reply(200, projectItem())
packageAdaptor.onGet(/\/api\/project\/list/).reply(config => {
  const {sortBy = [], sortDesc = [], itemsPerPage = 15} = config.params

  const items = _.orderBy(
    Array(itemsPerPage).fill(null).map((__, i) => projectItem(i)),
    sortBy,
    sortDesc.map(desc => desc ? 'desc' : 'asc'),
  )

  return [200, {total: itemsPerPage * 3 + 3, items}]
})


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const deviceItem = (id = 1, name) => ({
  id,
  name: name,
  time: (Math.random() * 30 + 40).toFixed(0),
  category: Math.random() > 0.5 ? 'MIT' : 'GNU',
  percent: (Math.random() * 30 + 40).toFixed(0),
  price: (Math.random() * 400 + 100).toFixed(0),
  occupy: Math.random() > 0.5,
  type: ['Dialog 70004', 'Analog Device', 'STM', 'interface', 'Data Collection'][`${Math.abs(Math.random() - 0.5)}`[2]],
  tags: ['dehumidification', 'blood circulation', 'sleep aid', 'ventilation', 'beauty'][`${Math.abs(Math.random() - 0.5)}`[2]],
  lastModifyTime: (function (d) {
    const Y = d.getFullYear()
    const M = d.getMonth() + 1
    const D = d.getDate()
    const H = d.getHours()
    const m = d.getMinutes()
    const s = d.getSeconds()
    const pad = num => num.toString().padStart(2, '0')
    return `${Y}-${pad(M)}-${pad(D)} ${pad(H)}:${pad(m)}:${pad(s)}`
  })(new Date()),
})

packageAdaptor.onPost(/\/api\/device/).reply(config => {
  let deviceList = store.state.devices
  if (Array.isArray(deviceList)) {
    deviceList.push(JSON.parse(config.data).name)
  } else {
    deviceList = [JSON.parse(config.data).name]
  }
  store.state.devices = deviceList
  return [200]
})
packageAdaptor.onPut(/\/api\/device/).reply(200)
packageAdaptor.onDelete(/\/api\/device\/\d+/).reply(config => {

  let deviceList = store.state.devices
  const index = parseInt(config.url.split('/')[2])
  deviceList.splice(index, 1)
  store.state.devices = deviceList

  return [200]
})

packageAdaptor.onGet(/\/api\/device\/item/).reply(config => {

  let deviceList = store.state.devices
  const index = parseInt(config.url.split('/')[3])

  return [200, deviceList[index]]
})

packageAdaptor.onGet(/\/api\/device\/list/).reply(config => {
  const {sortBy = [], sortDesc = [], itemsPerPage = 15} = config.params
  const k = store.state.devices
  const items = _.orderBy(
    Array(k.length).fill(null).map((__, i) => deviceItem(i, k[i])),
    sortBy,
    sortDesc.map(desc => desc ? 'desc' : 'asc'),
  )

  return [200, {total: itemsPerPage, items}]
})

