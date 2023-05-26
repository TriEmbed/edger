<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex offset-sm3 sm6 xs12>
        <v-btn class="mr-2" depressed tile @click="test1">
          test 1
        </v-btn>
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
                  v-text="pos[0]"
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
              v-model="pos[0]"
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
                  @click="decrement(0)"
                >
                  remove
                </v-icon>
              </template>

              <template #append>
                <v-icon
                  color="#005db5"
                  @click="increment(0)"
                >
                  add
                </v-icon>
              </template>
            </v-slider>
          </v-card-text>
        </v-card>
        <v-card>
          <v-toolbar
            dense
            flat
          >
            <v-toolbar-title>
              <span class="subheading">Rheostat One</span>
            </v-toolbar-title>
          </v-toolbar>

          <v-card-text>
            <v-row
              class="mb-4"
              justify="space-between"
            >
              <v-col class="text-left">
                <span
                  class="text-h3 font-weight-light"
                  v-text="pos[1]"
                />
                <span class="subheading font-weight-light mr-1">pos[1]</span>
              </v-col>
            </v-row>
            <v-slider
              v-model="pos[1]"
              always-dirty
              color="#005db5"
              max="1024"
              min="0"
              track-color="grey"
              @mouseup="post_duration"
            >
              <template #prepend>
                <v-icon
                  color="#005db5"
                  @click="decrement(1)"
                >
                  remove
                </v-icon>
              </template>
              <template #append>
                <v-icon
                  color="#005db5"
                  @click="increment(1)"
                >
                  add
                </v-icon>
              </template>
            </v-slider>
          </v-card-text>
        </v-card>
        <v-card>
          <v-toolbar
            dense
            flat
          >
            <v-toolbar-title>
              <span class="subheading">Rheostat Two</span>
            </v-toolbar-title>
          </v-toolbar>

          <v-card-text>
            <v-row
              class="mb-4"
              justify="space-between"
            >
              <v-col class="text-left">
                <span
                  class="text-h3 font-weight-light"
                  v-text="pos[2]"
                />
                <span class="subheading font-weight-light mr-1">pos[2]</span>
                <v-fade-transition>
                  <v-avatar
                    v-if="isOn"
                    :style="{
                      animationDuration: animationDuration
                    }"
                    class="mb-1 v-avatar--metronome"
                    color="red"
                    size="6"
                  />
                </v-fade-transition>
              </v-col>
              <v-col class="text-right" />
            </v-row>
            <v-slider
              v-model="pos[2]"
              always-dirty
              color="#005db5"
              max="1024"
              min="0"
              track-color="grey"
              @mouseup="post_duration"
            >
              <template #prepend>
                <v-icon
                  color="#005db5"
                  @click="decrement(2)"
                >
                  remove
                </v-icon>
              </template>
              <template #append>
                <v-icon
                  color="#005db5"
                  @click="increment(2)"
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
    pos: [44, 55, 66],

    interval: null,
    isOn: false,
  }),

  computed: {
    animationDuration (which) {

      return `${30 / this.pos[which]}s`
    },
  },

  mounted: function () {
    this.post_duration()
    this.post_state()
  },

  methods: {
    post_duration () {
      // this.$ajax
      //   .post('/api/v1/blinker/duration', {
      //     duration_ms: Math.round(60000 / this.steps)
      //   })
      //   .then(data => {
      //     console.log(data)
      //   })
      //   .catch(error => {
      //     console.log(error)
      //   })
    },
    post_state () {
      // this.$ajax
      //   .post('/api/v1/blinker/state', {
      //     state: this.isOn
      //   })
      //   .then(data => {
      //     console.log(data)
      //   })
      //   .catch(error => {
      //     console.log(error)
      //   })
    },
    decrement (which) {
      this.pos[which]--
      this.post_duration(which)
    },
    test1 () {
      console.log("button")
    },
    increment (which) {
      this.pos[which]++
      this.post_duration(which)
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
