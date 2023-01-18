import Vue from 'vue'

export const hiddenClsName = 'permission_forbidden';

(function () {
  const style = document.createElement('style')
  style.innerText = `.${hiddenClsName} { display: none !important; }`

  const [head] = document.getElementsByTagName('head')
  head.append(style)
})()

// TODO: code reuse

// All permissions listed must be included for the element to display
const hasPermission = {
  install (Vue) {
    Vue.directive('hasPermission', function (el, binding, vNode) {
      const permissions = vNode.context.$store.state.account.permissions
      const value = Array.isArray(binding.value) ? binding.value : binding.value.split(',')
      let flag = true
      for (let v of value) {
        if (!permissions.includes(v)) {
          flag = false
          break
        }
      }
      if (!flag) {
        el.classList.add(hiddenClsName)
      } else {
        el.classList.remove(hiddenClsName)
      }
    })
  },
}

// The element displays as long as any of the listed permissions are included
const hasAnyPermission = {
  install (Vue) {
    Vue.directive('hasAnyPermission', function (el, binding, vNode) {
      const permissions = vNode.context.$store.state.account.permissions
      const value = Array.isArray(binding.value) ? binding.value : binding.value.split(',')
      let flag = false
      for (let v of value) {
        if (permissions.includes(v)) {
          flag = true
          break
        }
      }
      if (!flag) {
        el.classList.add(hiddenClsName)
      } else {
        el.classList.remove(hiddenClsName)
      }
    })
  },
}

// All roles listed must be included for the element to display
const hasRole = {
  install (Vue) {
    Vue.directive('hasRole', function (el, binding, vNode) {
      const permissions = vNode.context.$store.state.account.roles
      const value = Array.isArray(binding.value) ? binding.value : binding.value.split(',')
      let flag = true
      for (let v of value) {
        if (!permissions.includes(v)) {
          flag = false
          break
        }
      }
      if (!flag) {
        el.classList.add(hiddenClsName)
      } else {
        el.classList.remove(hiddenClsName)
      }
    })
  },
}

// The element will display as long as it contains any of the listed roles
const hasAnyRole = {
  install (Vue) {
    Vue.directive('hasAnyRole', function (el, binding, vNode) {
      const permissions = vNode.context.$store.state.account.roles
      const value = Array.isArray(binding.value) ? binding.value : binding.value.split(',')
      let flag = false
      for (let v of value) {
        if (permissions.includes(v)) {
          flag = true
          break
        }
      }
      if (!flag) {
        el.classList.add(hiddenClsName)
      } else {
        el.classList.remove(hiddenClsName)
      }
    })
  },
};

[hasPermission, hasAnyPermission, hasRole, hasAnyRole].forEach(plugin => {
  Vue.use(plugin)
})

export {hasPermission, hasAnyPermission, hasRole, hasAnyRole}
