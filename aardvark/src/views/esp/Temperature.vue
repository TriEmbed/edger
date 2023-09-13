<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex offset-sm3 sm6 xs12 />
    </v-layout>

    <v-sparkline
      :gradient="['#f72047', '#ffd200', '#1feaea']"
      :line-width="2"
      :padding="8"
      :smooth="10"
      :value="get_chart_value"
      auto-draw
      gradient-direction="top"
      stroke-linecap="round"
    />
  </v-container>
</template>


<script>
import axios from "axios";
import temperatureActiveList from "@/store/modules/temperature"
export default {
  data: () => ({
    bpm: 120,
    interval: null,
    isOn: false,
  }),

  computed: {

    get_chart_value () {
      return this.$store.state.temperature.temperatureActiveList
    },


    animationDuration () {
      return `${30 / this.bpm}s`
    },
  },

  mounted: function () {

    clearInterval(this.timer);
    this.timer = setInterval(this.updateData, 5000);
  },

  methods: {
    updateData: function () {

      axios.get("http://esp-home.local/api/v1/temp/raw")
        .then(data => {
          this.$store.state.temperature.temperatureActiveList.push(data.data.raw)
        })
        .catch(error => {
          console.log(error);
        });exit
    },
  },
}

</script>

<style>
@keyframes metronome-example {
  from {
    transform: scale(0.2);
  }

  to {
    transform: scale(1);
  }
}

.v-avatar--metronome {
  animation-direction: alternate;
  animation-iteration-count: infinite;
  animation-name: metronome-example;
}
</style>
