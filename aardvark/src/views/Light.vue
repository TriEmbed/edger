<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-card>
        <v-color-picker 
          class="ma-2"
          v-model="color"
          canvas-height=300
          hide-mode-switch
          hide_inputs
          :show-swatches="false"
          @input="set_color"
        />
      </v-card>
    </v-layout>
  </v-container>
</template>

<script>
import axios from "axios";

export default {
  data () {
    return {color: '#001000', red: 0, green: 10, blue: 0};
  },
  methods: {
    set_color: function () {
      var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(this.color);
      this.red = parseInt(result[1], 16)
      this.green = parseInt(result[2], 16)
      this.blue = parseInt(result[3], 16)
      axios.defaults.baseURL = "http://" + "esp-home" + ".local";
      axios
        .post("/api/v1/light/brightness", {
          red: this.red,
          green: this.green,
          blue: this.blue,
        })
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log(error);
        });
    },
  },
};
</script>
