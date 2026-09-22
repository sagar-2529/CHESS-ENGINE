import Piece from './Piece';

export default function Square({ row, col, symbol, isLight, isSelected, isLastMove, inCheck, onClick }) {
  // Build class list
  const classes = [
    'sq',
    isLight ? 'sq-light' : 'sq-dark',
    !isSelected && !inCheck && (isLight ? 'sq-hover-light' : 'sq-hover-dark'),
    isLastMove && !isSelected && !inCheck ? 'sq-last-move' : '',
    isSelected ? 'sq-selected' : '',
    inCheck    ? 'sq-check'    : '',
  ].filter(Boolean).join(' ');

  const file = String.fromCharCode('a'.charCodeAt(0) + col);
  const rank = 8 - row;

  return (
    <div
      className={classes}
      onClick={onClick}
      role="button"
      aria-label={`${file}${rank}${symbol && symbol !== '.' ? ` ${symbol}` : ''}`}
      tabIndex={0}
      onKeyDown={e => e.key === 'Enter' && onClick()}
    >
      <Piece symbol={symbol} />
    </div>
  );
}
