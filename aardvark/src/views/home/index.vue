<template ref="here">
  <div>
    <v-toolbar flat="true" height="30" />
    <h4 id="scanned">
      looking for device on local network
    </h4>
    <h5 style="visibility: hidden;">
      {{ show }}// hack alert: necessary to update devices
    </h5>
    <DataTable
      :key="componentKey"
      ref="table"
      :default-options="{
        sortBy: ['lastModifyTime'],
        sortDesc: [true],
      }"
      :headers="headers"
      :load-data="loadData"
      item-key="id"
    >
      <template #[`item.number`]="{ index }">
        {{ index + 1 }}
      </template>

      <template #[`item.actions`]="{ item }">
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleConnect(item)" v-on="on">
              rowing
            </v-icon>
          </template>
          <span>connect</span>
        </v-tooltip>
        <v-tooltip top>
          <template #activator="{ on, attrs }">
            <v-icon class="mr-4" color="blue darken-3" v-bind="attrs" @click="handleEdit(item)" v-on="on">
              edit
            </v-icon>
          </template>
          <span>information</span>
        </v-tooltip>
      </template>
    </DataTable>

    <AntSchema
      ref="antSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>


import {deleteDevice} from '@/api/device'
import toast from '@/utils/toast'
import axios from 'axios'
import AntSchema from "@/views/home/modules/AntSchema.vue";
import {getInfo, getMenus} from "@/api/esp";
import store from "@/store";
import {AntMutations} from "@/store/modules/ant";
import {onActivated} from "vue";
import {toInteger} from "lodash-es";

let loop = -1
const loopMax = 30
const delayTime = 2000;
let mdnsNamesIndex = 0
let address = "Cat"
const scanMDNSAddress = () => {
// Get all connected USB devices the website has been granted access to.
//   navigator.usb.getDevices().then(devices => {
//     devices.forEach(device => {
//       console.log(device.productName);      // "Arduino Micro"
//       console.log(device.manufacturerName); // "Arduino LLC"
//     });
//   }).


  /* here we look for any esp32 that may be out there
 */
  const mdnsNames = ["esp-home"]

  const infoRequest = "/api/v1/system/info"
  // loop being -1 means stop
  if (loop === -1) {
    return
  }
  if (loop > loopMax) {
    if (mdnsNamesIndex >= mdnsNames.length - 1) {
      return;
    }
    mdnsNamesIndex++;
    loop = 0
  }
  let pass = toInteger(loop / 3)

  let mdnsName = mdnsNames[mdnsNamesIndex];
  if (loop == 0) {
    store.dispatch(`ants/${AntMutations.ANTS}`, null)
    loop = 0;

  } else {
    if (pass != 0) {
      mdnsName += "-" + pass
    }
  }
  loop++;
  // and the number to the address
  // not necessary for the first one
  address = mdnsName

  var s = document.getElementById('scanned');
  s.innerText = "looking for device on network " + mdnsName + ".local pass " + toInteger(loop % 3)
  axios.defaults.baseURL = "http://" + mdnsName + ".local";
  console.log("looking for", axios.defaults.baseURL)

  axios.get('/api/v1/system/info', {timeout: 1000})
    .then(response => {
      loop = 3 * toInteger((loop + 3) / 3)
      console.log("found", mdnsName)

      response.data["mdnsName"] = mdnsName
      store.dispatch(`ants/${AntMutations.ANTS}`, response.data).then(r => {
        console.log(axios.defaults.baseURL, response.data)
      }).catch((error) => {
        console.log("mutation error", error)
      })
    }).catch((error) => {
      console.log("not found", mdnsName)
    }).finally(() => {

      setTimeout(() => {
        scanMDNSAddress()
      }, 1000)
    })


};

export default {
  name: 'Home',
  components: {
    AntSchema,
  },
  computed: {
    show () {
      const data = this.$store.state.ants.antList
      if (data.length !== 0)
        this.forceRerender()
      return address
    },
    infoData () {
      return "hello";

    },
    headers () {
      return AntSchema.header
    },
  },

  data: () => ({
    items: [{title: 'rescan', action: "scanMDNSAddress"}],
    componentKey: 0,
    query: {
      name: '',
    },
  }),
  async loadGetInfo () {
    await getInfo(null)
  },
  async loadGetMenu () {
    //  await getMenus(null)
  },
  methods: {
    forceRerender () {
      this.componentKey += 1;
    },
    loadData: async function (options = {}) {
      return new Promise((resolve) => {
        const data = this.$store.state.ants.antList

        let dataLength = data.length

        if (dataLength === 0)
          return ({items: [], total: 0})
        dataLength = data.length
        console.log("scan new data", JSON.stringify(data[dataLength]));
        //     this.$refs['table'].refresh(true)
        resolve({items: data, total: dataLength})
      })
    },
    action (index) {
      // this.items[index].action()
      //console.log(index)
    },
    //
    // /**
    //  * Added items
    //  * @return {Undefined}
    //  */
    // testGetMenu (id) {
    //   this.loadGetMenu().then(a => alert(a))
    // },
    //
    //
    // async loadGetInfo () {
    //   await getInfo(null)
    // },


    // async loadData (options = {}) {
    //   return getDeviceList({...this.query, ...options}).then(r => r.data)
    // },
    /**
     * Added items
     * @return {Undefined}
     */
    reScan () {
      store.dispatch(`ants/${AntMutations.ANTS}`, null)
      scanMDNSAddress()
    },
    /**
     * Added device successfully
     * @return {Undefined}
     */
    handleAddSuccess () {
      toast.success({message: 'Add item successfully'})
      this.query = this.$options.data.apply(this).query
      this.$refs['table'].refresh(true)
    },
    /**
     * Edit item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleEdit (item) {

      console.log("AntSchema")
      //      let q = this.HomeItems.filter(d => d.tab === tab)[0].Schema

      this.$refs['antSchema'].open(item)
    },
    /**
     * connect item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleConnect (item) {

      // this call into device
      const addressMDNS = this.$store.state.ants.antList[item.id]["mdnsName"]
      console.log(addressMDNS)

      const routes = this.$router.getRoutes()

      const findDup = (index) => {
        if (name == null)
          return false
        return name.includes(addressMDNS)
      };
      const found = routes.find(route => findDup(route.name))

      if (found == null) {
        this.$store.state.ants.mDNSofCurrentAnt = addressMDNS;
        getMenus(addressMDNS)
      }
    },
    /**
     * Edit device success
     * @return {Undefined}
     */
    handleEditSuccess () {
      toast.success({message: 'Editing device successful'})
      this.$refs['table'].refresh()
    },
    /**
     * delete item
     * @param {Number | String} id item id
     * @return {Promise<Undefined>}
     */
    async handleDelete (id) {
      await deleteDevice(id)
      toast.success({message: 'Delete the item successfully'})
      await this.$refs.table.refresh()
    },


  },

  setup: function () {


    const testEvent = () => {
      // event handler
      document.addEventListener("index-of-event2", function (e) {
        console.log("3", e.detail); // Prints "Example of an event"
      });

      eval('document.addEventListener("index-of-event", function(e) {\n' +
          'console.log("1",e.detail); // Prints "Example of an event"\n' +
          'const event = new CustomEvent("index-of-event2", {"detail": "Example of an event"});' +
          'document.dispatchEvent(event);' +
          '})\n');

      navigator.usb.getDevices().then((devices) => {
        console.log(`Total devices: ${devices.length}`);
        devices.forEach((device) => {
          console.log(
            `Product index: ${device.productName}, serial number ${device.serialNumber}`,
          );
        });
      });

      document.addEventListener("index-of-event", function (e) {
        console.log("2", e.detail); // Prints "Example of an event"
      });
    };

    onActivated(async () => {
// not sure why this is needed, and it causes uncaught exceptions in navigator.usb.getDevices() above
//      testEvent()

      const event = new CustomEvent("index-of-event", {"detail": "Example of an event"});
      // Dispatch/Trigger/Fire the event
      document.dispatchEvent(event);

      setTimeout(() => {
        loop = 0;
        scanMDNSAddress()
      }, delayTime)
      this.componentKey += 1
    },
    )
  },


  /**
   * Added items
   * @return {Undefined}
   */
  testGetMenu () {
    this.loadGetMenu().then(
      a => {
        alert(a)
      },
    )
  }
  ,


}
</script>

<style lang="scss">
</style>
