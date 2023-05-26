<template>
  <v-container class="fill-height" fluid>
    <v-row align="center" justify="center">
      <v-col cols="12" md="4" sm="8">
        <v-form ref="form">
          <v-card class="elevation-12">
            <v-toolbar color="primary" dark flat>
              <v-toolbar-title>Login form</v-toolbar-title>
              <v-spacer />
            </v-toolbar>

            <v-card-text>
              <v-text-field
                v-model="formData.username"
                :rules="[v => !!v || 'please enter user index']"
                autofocus
                label="Login"
                name="login"
                prepend-icon="person"
                type="text"
                validate-on-blur
              />
              <v-text-field
                id="password"
                v-model="formData.password"
                :rules="[v => !!v || 'Please enter password']"
                label="Password"
                name="password"
                prepend-icon="lock"
                type="password"
                validate-on-blur
              />
            </v-card-text>

            <v-card-actions>
              <v-spacer />
              <v-btn
                :loading="loading"
                color="primary"
                type="submit"
                @click.prevent="handleSubmit"
              >
                Log in
              </v-btn>
            </v-card-actions>
          </v-card>
        </v-form>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import {mapActions} from 'vuex'
import {AccountActions} from '@/store/modules/account'

export default {
  name: 'Login',
  data: () => ({
    formData: {
      username: '',
      password: '',
    },
    loading: false,
  }),
  methods: {
    ...mapActions('account', {
      login: AccountActions.LOGIN,
    }),
    async handleSubmit () {
      if (!this.$refs.form.validate()) return
      try {
        this.loading = true
        await this.login(this.formData)
      } finally {
        this.loading = false
      }
    },
  },
  mounted () {
    setTimeout(() => {
      this.formData = {
        username: 'Admin',
        password: 'h97rpXts8@qzj7wp',
      }
    }, 450)
  },
}
</script>

<style lang="scss">

</style>
