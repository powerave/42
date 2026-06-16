<template>
  <footer class="bg-dark px-4 py-2 text-center small text-secondary">
    <RouterLink class="text-light no-underline" to="/privacy">{{ $t('footer.privacy') }}</RouterLink>
    <span class="mx-2">{{ $t('footer.separator') }}</span>
    <RouterLink class="text-light no-underline" to="/terms">{{ $t('footer.terms') }}</RouterLink>
  </footer>
</template>
