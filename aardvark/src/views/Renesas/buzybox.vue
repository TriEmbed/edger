<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-toolbar
            flat
            dense
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
                  v-text="steps1"
                />
                <span class="subheading font-weight-light mr-1">steps1</span>
              </v-col>
            </v-row>
            <v-slider
              v-model="steps1"
              color="#005db5"
              track-color="grey"
              always-dirty
              min="0"
              max="1024"
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
            flat
            dense
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
                  v-text="steps2"
                />
                <span class="subheading font-weight-light mr-1">steps2</span>
                <v-fade-transition>
                  <v-avatar
                    v-if="isOn"
                    color="red"
                    :style="{
                      animationDuration: animationDuration
                    }"
                    class="mb-1 v-avatar--metronome"
                    size="6"
                  />
                </v-fade-transition>
              </v-col>
              <v-col class="text-right" />
            </v-row>
            <v-slider
              v-model="steps2"
              color="#005db5"
              track-color="grey"
              always-dirty
              min="0"
              max="1024"
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
    steps1: 120,
    steps2: 120,
    interval: null,
    isOn: false,
  }),

  computed: {
    animationDuration (which) {

      return which==1?  `${30 / this.steps1}s`:  `${30 / this.steps2}s`
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
      if (which==1) {
        this.steps1--
      }else {
        this.steps1--
      }
      this.post_duration(which)
    },
    increment (which) {
      if (which==1) {
        this.steps1++
      } else
      {
        this.steps2++
      }
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
