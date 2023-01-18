<script>
import AppHeader from '@/layout/AppHeader.vue'
import AppContent from '@/layout/AppContent.vue'
import AppNavigation from '@/layout/AppNavigation.vue'
import AppSetting from '@/layout/AppSetting.vue'
import AppFooter from "@/layout/AppFooter";

export default {
  name: 'AppPage',
  render () {
    return (
      <div class="app-page">
        <AppNavigation/>
        <AppHeader/>
        <AppSetting/>
        <AppContent/>
        <AppFooter/>
      </div>
    )
  },
}
</script>

<style lang="scss">
.app-page {
  height: 100vh;
  width: 100vw;
}
</style>
