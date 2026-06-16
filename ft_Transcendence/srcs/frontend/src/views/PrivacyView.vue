<template>
  <div class="row justify-content-center g-3">
    <div class="col-12 col-xl-9">
      <div class="card bg-secondary text-white border border-secondary">
        <div class="card-header bg-success text-dark fw-bold text-uppercase">{{ $t('footer.privacy') }}</div>
        <div class="card-body d-flex flex-column gap-3">
          <p class="text-muted small">{{ $t('privacy.last_update') }}</p>

          <h5 class="text-success mb-0">{{ $t('privacy.section1_title') }}</h5>
          <p class="mb-0">{{ $t('privacy.section1_intro') }}</p>
          <ul>
            <li><strong>{{ $t('privacy.section1_item1_label') }}</strong> {{ $t('privacy.section1_item1_text') }}</li>
            <li><strong>{{ $t('privacy.section1_item2_label') }}</strong> {{ $t('privacy.section1_item2_text') }}</li>
            <li><strong>{{ $t('privacy.section1_item3_label') }}</strong> {{ $t('privacy.section1_item3_text') }}</li>
          </ul>

          <h5 class="text-success mb-0">{{ $t('privacy.section2_title') }}</h5>
          <p class="mb-0">{{ $t('privacy.section2_text') }}</p>

          <h5 class="text-success mb-0">{{ $t('privacy.section3_title') }}</h5>
          <p class="mb-0">{{ $t('privacy.section3_text') }}</p>

          <h5 class="text-success mb-0">{{ $t('privacy.section4_title') }}</h5>
          <p class="mb-0">{{ $t('privacy.section4_text') }}</p>
        </div>
      </div>
    </div>
  </div>
</template>