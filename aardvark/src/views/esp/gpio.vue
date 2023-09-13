<template>
  <div>
    <h4> {{ scanDataString }} </h4>
    <v-toolbar height="40">
      <p v-for="item in getToolbars">
        <v-btn
          variant="outlined"
          @click="action(item.action)"
        >
          {{ item.title }}
        </v-btn>
      </p>
    </v-toolbar>

    <div>
      <ProgramGPIO />
    </div>
  </div>
</template>


<script>

import store from "@/store";
import axios from "axios";
import {EspMutations} from "@/store/modules";

import ProgramGPIO from "@/components/ProgramGPIO/index.vue"

const example = [...Array(20).keys()]
  .map(i => `A${i} B${i} C${i} D${i} D${i}`)
  .join("\n");

const extractor = path => path + "oK";
const makeData = () => {
  let val = []

  // for each colum
  for (let colIndex = 0; colIndex < 16; colIndex++) {
    // the corner element id empty
    // fill the top row
    let rows = colIndex < 8 ? [colIndex.toString(16)] : [""]
    for (let rowIndex = 0; rowIndex < 16; rowIndex++) {
      rows.push(colIndex < 8 ? (colIndex * 16 + rowIndex).toString(16) : "");
    }
    val.push(rows)
  }
  //val.splice(8,8)
  return val;
};
export default {
  name: 'GPIO',
  components: {ProgramGPIO},

  data: () => ({
    selectedNumber: null,
    selected: '',
    width: 70,
    demoBasic: {
      format: [{noedit: true}],
      state: [[]],
      data: makeData(),
    },
    dataSrc: example,
    mode: "basic",
    edit: false,
    show: "scan",
  }),
  computed: {
    numberOptions () {
      let options = [];
      for (let i = 1; i <= 10; i++) {
        options.push(i);
      }
      return options;
    },
    scanList: function () {

      return this.$store.state.esp.scan
    },
    scanDataString: function () {
      const scan = this.$store.state.esp.scan
      console.log("scan new data", scan);
      scan.forEach(k => {
        const x = k % 16, y = k >> 4;
        this.$refs.grid.setDataAt(x + 1, y, k.toString(16));
      })
      return scan.toString()
    },
    gridData () {
      return this.dataSrc.split("\n").map(s => s.split(" "));
    },

    getToolbars () {
      let menuToSpliceIn = store.getters["account/getMenu"]

      const findPage = (target, menuToSpliceIn) => {

        for (const menu of menuToSpliceIn) {

          if (menu.text === target) {
            const subMenu = menu['subMenu']
            if (subMenu) {
              return subMenu
            }
          }
          if (menu.children) {
            const ret = findPage(target, menu.children)
            if (ret)
              return ret

          }
        }
        return null
      };
      console.log(menuToSpliceIn[1].text)
      return findPage('i2c', menuToSpliceIn)

    },

  },
  render () {
    const here = extractor(this.$route.path)

    return (
      <div class="home">
        <p>ESP {here}</p>
      </div>
    )
  }
  ,
  methods: {
    doScan: function (index) {
      const address = this.$route.meta['path'] + '?' + index + '='

      axios({method: 'patch', url: address, timeout: 1000, responseType: "json", mode: "no_cors"})
        .then((response) => {
          console.log("done axios")
          const val = response.data['i2c'][0]['scan']
          store.dispatch(`esp/${EspMutations.ESP_I2C_SCAN}`, val).then(r => {
            console.log("done store", val)
          }).catch((error) => {
            console.log("store error")
          })
          console.log(response.data)
        }).catch((error) => {
          console.log(error)
        })
      console.log(address)
    },
    action: function (index) {
      this.show = index
      switch (index) {

        case 'scan':

          this.doScan(index)
          break;
        default:
      }
    },
  },
}
</script>

<style lang="scss">
.home {
  height: 100%;
  position: relative;
  width: 100%;
}
</style>
