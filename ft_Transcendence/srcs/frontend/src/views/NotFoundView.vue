<script setup lang="ts"></script>

<template>
  <div class="row justify-content-center g-3">
    <div class="col-12 col-xl-6">
      <div class="alert alert-warning mb-0">
        {{ $t('notfound.prefix') }} <RouterLink to="/home" class="alert-link">{{ $t('sidebar.uno_home') }}</RouterLink>.
      </div>
    </div>
  </div>
</template>
