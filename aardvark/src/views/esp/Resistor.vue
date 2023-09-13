<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex offset-sm3 sm6 xs12>
        <v-card>
          <v-toolbar
            dense
            flat
          >
            <v-toolbar-title>
              <span class="subheading">Blinker speed</span>
            </v-toolbar-title>
          </v-toolbar>

          <v-card-text>
            <v-row
              class="mb-4"
              justify="space-between"
            >
              <v-col class="text-left">
                <span
                  class="text-h2 font-weight-light"
                  v-text="bpm"
                />
                <span class="subheading font-weight-light mr-1">BPM</span>
                <v-fade-transition>
                  <v-avatar
                    v-if="isOn"
                    :style="{
                      animationDuration: animationDuration
                    }"
                    class="mb-1 v-avatar--metronome"
                    color="red"
                    size="12"
                  />
                </v-fade-transition>
              </v-col>
              <v-col class="text-right">
                <v-btn
                  color="#005db5"
                  dark
                  depressed
                  fab
                  @click="toggle"
                >
                  <v-icon large>
                    {{ isOn ? "pause" : "play_arrow" }}
                  </v-icon>
                </v-btn>
              </v-col>
            </v-row>

            <v-slider
              v-model="bpm"
              always-dirty
              color="#005db5"
              max="600"
              min="10"
              track-color="grey"
              @mouseup="post_duration"
            >
              <template #prepend>
                <v-icon
                  color="#005db5"
                  @click="decrement"
                >
                  remove
                </v-icon>
              </template>

              <template #append>
                <v-icon
                  color="#005db5"
                  @click="increment"
                >
                  add
                </v-icon>
              </template>
            </v-slider>
          </v-card-text>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data: () => ({
    bpm: 120,
    interval: null,
    isOn: false,
  }),

  computed: {
    animationDuration () {
      return `${30 / this.bpm}s`
    },
  },

  mounted: function () {
    this.post_duration()
    this.post_state()
  },

  methods: {
    post_duration () {
      this.$ajax
        .post('/api/v1/blinker/duration', {
          duration_ms: Math.round(60000 / this.bpm),
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    post_state () {
      this.$ajax
        .post('/api/v1/blinker/state', {
          state: this.isOn,
        })
        .then(data => {
          console.log(data)
        })
        .catch(error => {
          console.log(error)
        })
    },
    decrement () {
      this.bpm--
      this.post_duration()
    },
    increment () {
      this.bpm++
      this.post_duration()
    },
    toggle () {
      this.isOn = !this.isOn
      this.post_state()
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
