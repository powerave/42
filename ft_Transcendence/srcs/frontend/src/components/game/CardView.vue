<script setup lang="ts">
import { computed } from 'vue';
import type { GameCard, CardColor } from '../../types/game';
import { getCardImageFilename } from '../../utils/cardMapping';

const props = defineProps<{
  card: GameCard | null;
  faceDown?: boolean;
  resolvedColour?: CardColor | null;
  disabled?: boolean;
  hoverable?: boolean;
}>();

const emit = defineEmits<{
  (e: 'click', card: GameCard | null): void;
}>();

// Vite exposes the "assets" folder as the public root folder (see vite.config.ts `publicDir: 'assets'`), 
// so the image URLs can be constructed directly from the card properties without needing to import each image.
const imagePath = computed(() => {
  const filename = getCardImageFilename(props.card, props.faceDown, props.resolvedColour);
  return `/Cards/${filename}`;
});

function onClick() {
  if (!props.disabled) {
    emit('click', props.card);
  }
}
</script>

<template>
  <div 
    class="uno-card-wrapper" 
    :class="{ 
      'is-disabled': disabled, 
      'is-hoverable': hoverable && !disabled 
    }"
    @click="onClick"
  >
    <img 
      :src="imagePath" 
      :alt="faceDown ? 'Dos de carte' : `${card?.colour} ${card?.value}`" 
      class="uno-card-img"
    />
  </div>
</template>

<style scoped>
.uno-card-wrapper {
  display: inline-block;
  transition: transform 0.2s ease, opacity 0.2s ease, box-shadow 0.2s ease;
  user-select: none;
}

.uno-card-img {
  width: 100%;
  height: auto;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
}

.is-hoverable {
  cursor: pointer;
}

.is-hoverable:hover {
  transform: translateY(-10px);
  box-shadow: 0 8px 12px rgba(0, 0, 0, 0.5);
}

.is-disabled {
  opacity: 0.5;
  cursor: not-allowed;
}
</style>