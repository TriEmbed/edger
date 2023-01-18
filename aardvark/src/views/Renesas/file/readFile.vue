<template>
  <div id="app">
    <h1>readFile (active i2c)</h1>
    <select v-model="mode">
      <option value="basic">
        Basic
      </option>
      <option value="csv">
        CSV Editing
      </option>
    </select>
    <v-btn @click="flashRam">
      Flash Ram
    </v-btn>
    <v-btn @click="flashEEPROM">
      EEPROM Ram
    </v-btn>
    <div
        class="drop"
        :class="getClasses"
        @dragover="dragOver"
        @prevent="dragLeave"
        @drop="drop($event)"
        v-on:click.native.prevent
    >
      <!--      <textarea v-model="textSource" v-if="textSource"/>-->
      <!--      <h1 v-if="wrongFile">-->
      <!--        Wrong file type-->
      <!--      </h1>-->
      <div>
        <!--        <h1 v-if="!textSource && !isDragging && !wrongFile">-->
        <!--          Drop <label for="uploadmytextfile">(or pick)</label> a text file-->
        <!--        </h1>-->
      </div>
      <!--      <input type="file" id="uploadmytextfile" @change="requestUploadFile">-->
      <div v-if="mode === 'basic'">
        <vue-grid-sheet
            ref="grid"
            :data="demoBasic.data"
            :format="demoBasic.format"
            :header="demoBasic.header"
            :state="demoBasic.state"
        />
      </div>
      <div v-if="mode === 'csv'" class="pane">
        <textarea v-model="dataSrc"/>
        <div class="right">
          <vue-spreadsheet-lite :data="gridData" @update="onUpdate"/>
        </div>
      </div>
    </div>
  </div>
</template>

<script>


import VueGridSheet from "@/components/gridsheet/gridsheet";
import {set} from "@/api/esp"
import xml2js from "xml2js";

const example = [...Array(20).keys()]
    .map(i => `A${i} B${i} C${i} D${i} D${i}`)
    .join("\n");


let width = 50;


const makeData = () => {
  let val = []
  for (let ci = 0; ci < 16; ci++) {
    let e = [ci.toString(16)]
    for (let j = 0; j < 16; j++) {
      e.push((ci * 16 + j).toString(16));
    }
    val.push(e)
  }
  return val;
};


export default {
  name: "App",
  components: {
    VueGridSheet,
  },

  computed: {
    gridData () {
      return this.dataSrc.split("\n").map(s => s.split(" "));
    },


    getClasses () {
      return {isDragging: this.isDragging}
    },
  },
  setup () {
    let header = [];
    header.push({name: " ", width: width});
    for (let i = 1; i < 16; i++) {
      header.push({name: i.toString(16), width: width});
    }
    console.log(header)
    width = 50;
    const count = 17;
    // expose to template and other options API hooks
    return {
      count, header,
    }
  },

  getFileName () {
    return this.fileContent ? (this.fileName || 're-upload') : 'upload'
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

  methods: {

    delayWrite (address, index) {

      console.log("delay thing", index)
      let val = this.$refs.grid.data[index]
      if (val) {

        val = val.slice(1, 17)
        console.log(val)
        let f = [];
        val.forEach(value => {
          f.push(parseInt(value, 16))
        })
        console.log(f);

        set(address, index * 16, f)

        setTimeout(this.delayWrite(address, index + 1), 10000);
      } else {
        makeData()
      }
    },
    flashRam () {


      setTimeout(this.delayWrite(8, 0), 10000);

    },
    flashEEPROM () {
      setTimeout(this.delayWrite(9, 0), 10000);
      this.demoBasic.state = [[]]
    },
    dragOver (ev) {
      console.log(" dragOver ()")
      this.isDragging = true

      ev.preventDefault()
    },
    dragLeave () {
      console.log(" dragLeave () ")
      this.isDragging = false
    },
    drop (ev) {

      console.log("drop (ev) ", ev)
      let files = ev.dataTransfer.files
      this.process(files)
      ev.preventDefault()
    },
    requestUploadFile () {
      var src = this.$el.querySelector('#uploadmytextfile')
      let files = src.files
      this.process(files)
    },
    process (files) {
      this.wrongFile = false


      const uploadFunc = (file, where) => {

        const xml2js = require('xml2js');
        const parser = new xml2js.Parser();


        const reader = new FileReader();
        reader.onload = (e) => {
          // this.$emit('input', reader.result)
          parser.parseString(reader.result, function (err, result) {
            const nvmData = result.GPDProject.chip[0].nvmData[0];
            // there are better was of doing this I am sure but this is quick hack
            const str = JSON.stringify(nvmData, null, 2).replace("\"_\"", "\"tmp\"");
            const val = JSON.parse(str).tmp.split(" ").map(value => value.toString(16));

            let index = 0;
            for (let ci = 0; ci < 16; ci++) {
              for (let ri = 1; ri < 17; ri++) {
                where.setDataAt(ri, ci, val[index++].toString(16))
              }
            }
          })

        }
        reader.readAsText(file)
      };

      // allows only 1 file
      if (files.length === 1) {
        let file = files[0]
        // allows text only
        uploadFunc(file, this.$refs.grid)
        // if (file.type.indexOf('text/') >= 0) {
        //   var reader = new FileReader()
        //   reader.onload = f => {
        //     this.textSource = f.target.result
        //     this.isDragging = false
        //   }
        //   // this is the method to read a text file content
        //   reader.readAsText(file)
      } else {
        this.wrongFile = true
        this.textSource = null

      }
    },
  },
  uploadFunc (fileName) {

  },

  getDimensions () {
    // this. width = document.documentElement.clientWidth/16;
  }
  ,

  onUpdate (data) {
    if (data) {
      let output = data.map(s => s.join(" ")).join("\n");
      this.dataSrc = output;
    }
  }
  ,

  data: () => ({
    width: 70,
    demoBasic: {
      format: {noedit: true},
      header: [],
      state: [[]],
      data: makeData(),
      isDragging: false,
      wrongFile: false,
      textSource: null,
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
  width: 20;
}

.right {
  flex: 1;
}
</style>

<style scoped>
.drop {
  align-items: center;
  background-color: #eee;
  border: 10px solid #eee;
  display: flex;
  font-family: sans-serif;
  height: 100%;
  justify-content: center;
  padding: 1rem;
  transition: background-color 0.2s ease-in-out;
  width: 100%;
}

.isDragging {
  background-color: #999;
  border-color: #fff;
}

textarea {
  height: 100%;
  object-fit: contain;
  resize: none;
  width: 100%;
}

input[type='file'] {
  display: none;
}

label {
  text-decoration: underline;
}
</style>
