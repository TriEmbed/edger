<template ref="here">
  <div>
    <v-toolbar flat="true" height="30">
      <v-btn
        v-for="(item, index) in items"
        :key="index"
        variant="outlined"
        @click="action(index)"
      />
    </v-toolbar>
    <h4 hidden>
      {{ show }}
    </h4>
    <DataTable

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

    <I2CSchema
      ref="i2cSchema"
      @addSuccess="handleAddSuccess"
      @editSuccess="handleEditSuccess"
    />
  </div>
</template>

<script>


import {deleteDevice} from '@/api/device'
import toast from '@/utils/toast'
import axios from 'axios'
import I2CSchema from "@/views/i2c/modules/I2CSchema.vue";
import {getInfo} from "@/api/esp";
import store from "@/store";

let loop = -1
const loopMax = 2
const delayTime = 500;
let mdnsNamesIndex = 0

const i2CFunction1 = (mdnsName, myParm) => {


  const u = new URLSearchParams(JSON.stringify(myParm)).toString()
  console.log(u)

  const infoRequest = "http://" + mdnsName + ".local/api/v1/i2c?get=" + u
  axios.patch(infoRequest, {timeout: delayTime}).then((response) => {
    console.log("0x" + response.data["i2c"][0]['get'][0].toString(16))
  })
}

//  curl --location --globoff --request PATCH 'http://esp-home.local/api/v1/i2c?get={%22address%22%3A72%2C%22index%22%3A11%2C%22length%22%3A1}' \
//--header 'Content-Type: text/plain' \
//--data 'asdasd'

const i2CFunction = () => {
// Get all connected USB devices the website has been granted access to.
//   navigator.usb.getDevices().then(devices => {
//     devices.forEach(device => {
//       console.log(device.productName);      // "Arduino Micro"
//       console.log(device.manufacturerName); // "Arduino LLC"
//     });
//   })
  const openDevice = (mdnsName) => {
    const fullAddress = "http://" + mdnsName + ".local"
    return axios.create({
      baseURL: fullAddress, timeout: delayTime,
      responseType: "json",
      mode: "no_cors",
      headers: {},
    });
  };


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
  let mdnsName = mdnsNames[mdnsNamesIndex]
  // and the number to the address
  // not necessary for the first one
  mdnsName = loop == 0 ? mdnsName : mdnsName + '-' + loop
  const files = openDevice(mdnsName)

  loop++
  files.get(infoRequest, {timeout: delayTime}).then((response) => {
    console.log("found", mdnsName)

    response.data["mdnsName"] = mdnsName
    store.dispatch(`ants/${AntMutations.ANTS}`, response.data).then(r => {
      console.log(response.data)
    }).catch((error) => {
      console.log("mutation error", error)
    })
  }).catch((error) => {
    console.log("not found", mdnsName)
  }).finally(() => {
    setTimeout(() => {
      i2CFunction()
    }, delayTime)
  })


};

export default {
  name: 'Home',
  components: {
    I2CSchema,
  },
  computed: {
    show () {
      const data = this.$store.state.ants.antList
      if (data.length !== 0)
        this.forceRerender()
      return data.length
    },
    infoData () {
      return "hello";

    },
    headers () {
      return I2CSchema.header
    },
  },

  data: () => ({
    items: [{title: 'rescan', action: "i2CFunction"}],
    componentKey: 0,
    query: {
      index: '',
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
        try {

          const data = I2CSchema.dataTest().formData
          const ants = this.$store.state.ants.antList
          let mDNS = this.$store.state.ants.mDNSofCurrentAnt
          if (mDNS === "") {
            mDNS = ants[0].mdnsName
            // this.$store.state.ants.mDNSofCurrentAnt = mDNS
          }


          const ant = ants.find(ant => ant.mdnsName === mDNS)
          const devices = ant.interface[2].devices;
          devices.forEach(device => {
            console.log(Object.keys(device),Object)
            for(let faces of Object.values(device)) {
              for (let face of faces)
              {
                console.log(Object.keys(device), face.name)

                data.push({index: Object.keys(device), Name: face.name})
              }
            }
          },
          )



          let dataLength = data.length

          if (dataLength === 0)
            return ({items: [], total: 0})
          dataLength = data.length
          console.log("scan new data", JSON.stringify(data[dataLength]));
          //     this.$refs['table'].refresh(true)
          resolve({items: data, total: dataLength})

        }

        catch(error)
        {
          console.log(error)
        }
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
      loop = 0;
      i2CFunction()
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

      console.log("I2CSchema")
      //      let q = this.HomeItems.filter(d => d.tab === tab)[0].Schema

      this.$refs['i2cSchema'].open(item)
    },
    /**
     * connect item
     * @param {Number | String} id item id
     * @return {Undefined}
     */
    handleConnect (item) {
      console.log(item.id)
      // this call into device
      //     const addressMDNS = this.$store.state.ants.mDNSofCurrentAnt
      //   console.log(addressMDNS)

      // i2CFunction1("esp-home",item.Param);

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
