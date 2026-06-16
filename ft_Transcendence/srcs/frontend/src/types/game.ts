export type CardColor = 'Red' | 'Green' | 'Yellow' | 'Blue' | 'Wild';

export type CardValue =
  | '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
  | 'Draw Two' | 'Skip' | 'Reverse'
  | 'Any' | 'Draw Four';

export interface GameCard {
  id?: number;
  colour: CardColor;
  value: CardValue;
}

export interface GamePlayerState {
  id: number;
  player_number: number;
  hand_size: number;
  result: 'win' | 'loss' | null;
}

export interface GameState {
  game_id: number;
  status: 'waiting' | 'playing' | 'finished';
  current_turn: number;
  direction: number;
  current_colour: CardColor | null;
  current_value: CardValue | null;
  deck_size: number;
  top_discard: GameCard | null;
  players: GamePlayerState[];
}