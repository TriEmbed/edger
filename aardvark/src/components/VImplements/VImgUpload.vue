<template>
  <!-- auto-upload -->
  <!-- action="//jsonplaceholder.typicode.com/posts/" -->
  <VueImgInputer
    ref="vueImgInputer"
    v-model="file"
    :class="flat ? 'elevation-0' : ''"
    :img-src="imgSrc"
    :max-size="maxSize"
    :no-hover-effect="readonly"
    :on-error="onUploadError"
    :on-progress="onUploadSProgress"
    :on-start="onUploadStart"
    :on-success="onUploadSuccess"
    :placeholder="`${placeholder}（maximun${maxMB}M）`"
    :readonly="readonly"
    :size="size"
    accept="image/*"
    icon="img"
    readonly-tip-text=""
    theme="material"
    @onChange="fileChange"
    @onExceed="overMaxSize"
  />
</template>

<script>
import 'vue-img-inputer/dist/index.css'
import VueImgInputer from 'vue-img-inputer'
import toast from '@/utils/toast'

export default {
  name: 'VImgUpload',
  components: {
    VueImgInputer,
  },
  props: {
    flat: {
      type: Boolean,
      default: false,
    },
    imgSrc: {
      type: String,
      default: '',
    },
    maxSize: {
      type: Number,
      default: 1024,
    },
    placeholder: {
      type: String,
      default: 'Click or drag to upload images',
    },
    readonly: {
      type: Boolean,
      default: false,
    },
    size: {
      type: String,
      default: 'normal',
      validator: size => ['small', 'normal', 'large'].includes(size),
    },
  },
  data: () => ({
    file: null,
  }),
  computed: {
    maxMB () {
      return this.maxSize / 1024
    },
  },
  methods: {
    /*** The file exceeds the specified size
     * Fired before fileChange

     * @param {File} e

     * @event

     */

    overMaxSize () {
      toast.error({message: `Image size exceeds ${this.maxMB}mega`})
      this.reset()

    },

    /**

     * select file

     * Triggered after overMaxSize

     * @param {File} e

     * @event

     */ fileChange (e) {
      if (!/image/.test(e.type)) {
        toast.error({message: 'Upload a file that is not an image'})
        this.reset()

      }
    },
    onUploadStart () {
    },
    onUploadSProgress () {
    },
    onUploadSuccess () {
    },
    onUploadError () {
    },
    reset () {
      this.file = null
      this.$refs['vueImgInputer'].reset()
      this.$refs['vueImgInputer'].$refs['inputer'].value = ''
    },
  },
}
</script>

<style lang="scss">
</style>
