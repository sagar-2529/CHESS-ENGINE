// Unicode chess pieces — works without any external assets
const UNICODE = {
  K: '♔', Q: '♕', R: '♖', B: '♗', N: '♘', P: '♙',  // White
  k: '♚', q: '♛', r: '♜', b: '♝', n: '♞', p: '♟',  // Black
};

export default function Piece({ symbol }) {
  if (!symbol || symbol === '.') return null;

  const unicode = UNICODE[symbol];
  if (!unicode) return null;

  const isWhite = symbol === symbol.toUpperCase();

  return (
    <span
      className={`piece ${isWhite ? 'piece-white' : 'piece-black'}`}
      aria-hidden="true"
    >
      {unicode}
    </span>
  );
}
