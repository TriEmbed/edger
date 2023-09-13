<template>
  <v-list class="py-0 recursive-menus">
    <template v-for="item in items">
      <!-- / branch nodes -->
      <v-list-group
        v-if="item.type === 'MENU' && (!item.hidden || showHidden)"
        :key="item.to"
        v-hasPermission="item.permissions"
        :class="sub ? 'group-sub' : ''"
        :value="$route.path.includes(item.to)"
        active-class="primary--text"
      >
        <template #prependIcon>
          <v-icon>
            {{ item.icon }}
          </v-icon>
        </template>
        <template #activator>
          <v-list-item-title>
            {{ item.text }}
          </v-list-item-title>
        </template>
        <LayoutRecursiveMenus :showHidden=showHidden v-if="item.children.length" :items="item.children" sub />
      </v-list-group>

      <!-- / leaf nodes -->
      <v-list-item
        v-if="item.type === 'VIEW' && (!item.hidden || showHidden)"
        :key="item.to"
        v-hasPermission="item.permissions"
        :to="item.to || item.redirect"
        active-class="primary--text"
        link
      >
        <v-list-item-icon>
          <v-icon>{{ item.icon }}</v-icon>
        </v-list-item-icon>
        <v-list-item-title>
          {{ item.text }}
        </v-list-item-title>
      </v-list-item>
    </template>
  </v-list>
</template>

<script>
export default {
  name: 'LayoutRecursiveMenus',
  props: {
    items: {
      type: Array,
      default: () => [],
    },
    sub: {
      type: Boolean,
      default: false,
    },
    showHidden: {
      type: Boolean,
      default: false,
    }
  },
}
</script>

<style lang="scss">
.group-sub {
  padding-left: 1rem;

  /* stylelint-disable-next-line */
  & > .v-list-group__items {
    padding-left: 1rem;
  }
}
</style>
