<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex offset-sm3 sm6 xs12>
        <v-card>
          <v-responsive :style="{ background: `rgb(${red}, ${green}, ${blue})` }" height="300px" />
          <v-card-text>
            <v-container fluid grid-list-lg>
              <v-layout row wrap>
                <v-flex xs9>
                  <v-slider v-model="red" :max="255" label="R" @input="set_color" />
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="red" class="mt-0" type="number" />
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="green" :max="255" label="G" @input="set_color" />
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="green" class="mt-0" type="number" />
                </v-flex>
                <v-flex xs9>
                  <v-slider v-model="blue" :max="255" label="B" @input="set_color" />
                </v-flex>
                <v-flex xs3>
                  <v-text-field v-model="blue" class="mt-0" type="number" />
                </v-flex>
              </v-layout>
            </v-container>
          </v-card-text>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
import axios from "axios";

export default {
  data () {
    return {red: 160, green: 160, blue: 160};
  },
  methods: {
    set_color: function () {
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
