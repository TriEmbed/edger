<template>
  <v-container fluid>
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
export default {
  data () {
    return {
      timer: null,
    };
  },
  computed: {
    get_chart_value () {
      return this.$store.state.chart_value;
    },
  },
  methods: {
    updateData: function () {
      this.$store.dispatch("update_chart_value");
    },
  },
  mounted () {
    clearInterval(this.timer);
    this.timer = setInterval(this.updateData, 1000);
  },
  destroyed: function () {
    clearInterval(this.timer);
  },
};
</script>
