<template>
  <div id="app">
    <h3>Scan (active i2c)</h3>
    <v-btn @click="scani2c">
      scani2c
    </v-btn>
    <h4> {{ infoData }} </h4>
    <select v-model="mode">
      <option value="csv">
        CSV Editing
      </option>
    </select>
    <div v-if="mode === 'basic'">
      <vue-grid-sheet
        ref="grid"
        :data="demoBasic.data"
        :format="demoBasic.format"
        :state="demoBasic.state"
      />
    </div>
    <div v-if="mode === 'csv'" class="pane">
      <textarea v-model="dataSrc" />
      <div class="right">
        <vue-spreadsheet-lite :data="gridData" @update="onUpdate" />
      </div>
    </div>
  </div>
</template>

<script>


import VueGridSheet from "@/components/gridsheet/gridsheet";
import {getPatch} from "@/api/esp"

const example = [...Array(20).keys()]
  .map(i => `A${i} B${i} C${i} D${i} D${i}`)
  .join("\n");


// let width = 50;


const makeData = () => {
  let val = []
  // for each colum
  for (let colIndex = 0; colIndex < 16; colIndex++) {
    // the corner element id empty
    // fill the top row
    let rows = colIndex < 8 ? [colIndex.toString(16)] : [""]
    for (let rowIndex = 0; rowIndex < 16; rowIndex++) {
      rows.push(colIndex < 8 ? '0x'+(colIndex * 16 + rowIndex).toString(16) : "");
    }
    val.push(rows)
  }
  //val.splice(8,8)
  return val;
};

export default {
  name: "Scan",
  components: {
    VueGridSheet,
  },

  computed: {

    infoData: function () {
      console.log("scan new data", this.$store.state.esp.info);
      return this.$store.state.esp.info.toString()
    },
    gridData () {
      return this.dataSrc.split("\n").map(s => s.split(" "));
    },

  },
  watch: {
    // this
    '$store.state.esp.scan': function (scans) {
      this.$refs.grid. clearSelection () ;
      scans.forEach(k => {
        const x = k % 16, y = k >> 4;
        this.$refs.grid.setDataAt(x + 1, y, +k.toString(10));
      })
    },
  },
  test () {

  },


  setup () {

    //const count = 17;
    // expose to template and other options API hooks
    // return {
    // count,
    // }
  },


  mounted: function () {

    // console.log(this.$store.state.esp.scan )
    // console.log(this.count)

    console.log("mounted app");


    // await store.dispatch("espEspMutations.ESP_INFO");
    //c
    // console.log('inner width is: ' + divA.clientWidth);
    // width=divA.clientWidth/17;
    // console.log('inner width is: ' + this.width);
    window.addEventListener('resize', this.getDimensions);
  }
  ,
  unmounted () {
    // window.removeEventListener('resize', this.getDimensions);
  }
  ,
  //  watch :{
  // "esp/${EspMutations.ESP_INFO}": function (){
  //      debugger
  //    },
  //  },

  activated: function () {
    //
    console.log("activated()")
  },
  deactivated: function () {
    console.log("deactivate()")
  },
  methods: {
    scani2c: function (event) {
      this.demoBasic.data = makeData();
      getPatch({info: '', scan: ''}, {})


    },

    getDimensions () {
      // this. width = document.documentElement.clientWidth/16;
    },

    onUpdate (data) {
      if (data) {
        let output = data.map(s => s.join(" ")).join("\n");
        this.dataSrc = output;
      }
    }
    ,
  }
  ,
  data: () => ({
    width: 70,
    demoBasic: {
      format: [{noedit: true}],
      state: [[]],
      data: makeData(),
    },
    dataSrc: example,
    mode: "basic",
    edit: false,
  }),
}
;
</script>

<style>
.pane {
  display: flex;
}

#app {
  margin: 1rem;
}

textarea {
  background: #333;
  color: rgb(128, 128, 128);
  width: 20px;
}

.right {
  flex: 1;
}
</style>
