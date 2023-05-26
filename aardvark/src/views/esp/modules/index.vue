<template>
  <FormDrawer
    v-model="visible"
    :loading="loading"
    :width="680"
    flat
    title="Edit item"
  >
    <template #content>
      <v-form ref="form">
        <v-container>
          <v-row>
            <v-col cols="12">
              <v-text-field
                v-model="formData.name"
                :rules="[v => !!v || 'Please enter a device index']"
                label="Device index"
              />
            </v-col>
          </v-row>

          <v-row>
            <v-col cols="6">
              <v-select
                v-model="formData.type"
                :items="['Foot Dao', 'Body Massage', 'Traditional Chinese Medicine', 'SPA', 'Package']"
                :rules="[v => !!v || 'Please select an item type']"
                label="Item Type"
              />
            </v-col>
            <v-col cols="6">
              <v-radio-group
                v-model="formData.category"
                :items="['Public Items', 'Other Items']"
                :rules="[v => !!v || 'Please select an item category']"
                label="Item Category"
                row
              >
                <v-radio color="primary" label="Public Items" value="Public Items" />
                <v-radio color="primary" label="Other items" value="Other items" />
              </v-radio-group>
            </v-col>
          </v-row>
          <v-col cols="6">
            <v-text-field
              v-model.number="formData.price"
              :rules="[v => !!v || 'Please enter a display price']"
              label="author"
            />
          </v-col>
          <v-col cols="6">
            <v-text-field
              v-model.number="formData.time"
              :rules="[v => !!v || 'Please enter the total duration']"
              label="Total duration (minutes)"
            />
          </v-col>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model.number="formData.percent"
                :rules="[v => !!v || 'Please enter the cost ratio']"
                label="Rating (%)"
                type="number"
              />
            </v-col>
            <v-col cols="6">
              <v-radio-group
                v-model="formData.occupy"
                :rules="[v => typeof v === 'boolean' || 'Please choose an exclusive room']"
                color="primary"
                label="GitHub Source Code"
                row
              >
                <v-radio :value="false" color="primary" ilabel="No" />
                <v-radio :value="true" color="primary" label="yes" />
              </v-radio-group>
            </v-col>
          </v-row>
          <v-row>
            <v-col
              class="py-0"
              cols="12"
            >
              <v-text-field
                v-model="formData.tags"
                :rules="[
                  v => !!v || 'Please enter the efficacy label',
                  v => v && v.length <= 12 || 'Enter up to 12 characters'
                ]"
                counter="12"
                label="Efficacy label"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="12">
              <VImgUpload ref="upload" flat />
            </v-col>
          </v-row>
        </v-container>
      </v-form>
    </template>

    <template #actions>
      <v-spacer />
      <v-btn
        text
        x-large
        @click="close"
      >
        Cancel
      </v-btn>
      <v-btn
        text
        type="submit"
        x-large
        @click.stop.prevent="submit"
      >
        save
      </v-btn>
    </template>
  </FormDrawer>
</template>

<script>

import {addDevice, editDevice, getDevice} from '@/api/device'
import _ from 'lodash-es'

export default {
  name: 'DeviceSchema',
  props: {},
  data: () => ({
    formData: {
      id: '',
      name: '',
      type: '',
      category: '',
      price: '',
      time: '',
      percent: '',
      occupy: null,
      tags: '',
    },

    loading: false,
    visible: false,
  }),
  methods: {
    async add () {
      await addDevice(this.formData)
      this.$emit('addSuccess')
    },
    async edit () {
      await editDevice(this.formData)
      this.$emit('editSuccess')
    },
    async open (id) {
      try {
        this.visible = true
        this.loading = true
        if (id) {
          const {data} = await getDevice(id)
          this.formData = _.pick(data, Object.keys(this.formData))
        }
      } finally {
        this.loading = false
      }
    },
    async close () {
      this.visible = false
      await this.$nextTick()
      Object.assign(this, this.$options.data.apply(this))
      this.$refs['form'].resetValidation()
      this.$refs['upload'].reset()
    },
    async submit () {
      if (!this.$refs['form'].validate()) return
      try {
        this.loading = true
        await this.formData.id ? this.edit() : this.add()
        this.close()
      } finally {
        this.loading = false
      }
    },
  },
}
</script>

<style lang="scss">
</style>
