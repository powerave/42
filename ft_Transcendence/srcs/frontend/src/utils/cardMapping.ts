import type { GameCard, CardColor } from '../types/game';

/**
 * Convert card data into the corresponding image filename for display purposes.
 * 
 * @param card The card to display
 * @param faceDown If true, displays the back of the card
 * @param resolvedColour The color chosen for a wild card (useful for the discard pile)
 * @returns The name of the PNG file (e.g., 'Red_7.png')
 */
export function getCardImageFilename(
  card: GameCard | null,
  faceDown: boolean = false,
  resolvedColour?: CardColor | null
): string {
  if (faceDown || !card) {
    return 'Back.png';
  }

  // wild cards have special handling
  if (card.colour === 'Wild') {
    if (card.value === 'Draw Four') {
      return 'Black_Plus_4.png';
    }
    
    // change color
    if (resolvedColour && resolvedColour !== 'Wild') {
      return `Change_Color_${resolvedColour}.png`;
    }
    
    return 'Change_Color_Black.png';
  }

// For other cards, we can directly map the color and value to the filename
  let valueStr = card.value as string;
  switch (card.value) {
    case 'Draw Two':
      valueStr = 'Plus_2';
      break;
    case 'Skip':
      valueStr = 'Block';
      break;
    case 'Reverse':
      valueStr = 'Reverse';
      break;
  }

  return `${card.colour}_${valueStr}.png`;
}
