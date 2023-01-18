<script>

const extractor = path => path + "oK";

export default {
  name: 'EspIndo',

  render () {

    const here = extractor(this.$route.path)

    return (
      <div class="home">
        <p>ESP {here}</p>
      </div>
    )
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
