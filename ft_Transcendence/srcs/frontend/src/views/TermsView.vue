<template>
  <div class="row justify-content-center g-3">
    <div class="col-12 col-xl-9">
      <div class="card bg-secondary text-white border border-secondary">
        <div class="card-header bg-success text-dark fw-bold text-uppercase">{{ $t('footer.terms') }}</div>
        <div class="card-body d-flex flex-column gap-3">
          <p class="text-muted small">{{ $t('terms.last_update') }}</p>

          <h5 class="text-success mb-0">{{ $t('terms.section1_title') }}</h5>
          <p class="mb-0">{{ $t('terms.section1_text') }}</p>

          <h5 class="text-success mb-0">{{ $t('terms.section2_title') }}</h5>
          <p class="mb-0">{{ $t('terms.section2_text') }}</p>

          <h5 class="text-success mb-0">{{ $t('terms.section3_title') }}</h5>
          <p class="mb-0">{{ $t('terms.section3_intro') }}</p>
          <ul>
            <li>{{ $t('terms.section3_item1') }}</li>
            <li>{{ $t('terms.section3_item2') }}</li>
            <li>{{ $t('terms.section3_item3') }}</li>
          </ul>

          <h5 class="text-success mb-0">{{ $t('terms.section4_title') }}</h5>
          <p class="mb-0">{{ $t('terms.section4_text') }}</p>
        </div>
      </div>
    </div>
  </div>
</template>