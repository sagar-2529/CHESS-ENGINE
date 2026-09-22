// PromotionDialog — polished modal for pawn promotion piece selection

const OPTIONS = [
  { value: 'q', label: 'Queen', white: '♕', black: '♛' },
  { value: 'r', label: 'Rook', white: '♖', black: '♜' },
  { value: 'b', label: 'Bishop', white: '♗', black: '♝' },
  { value: 'n', label: 'Knight', white: '♘', black: '♞' },
];

export default function PromotionDialog({ turn, onChoose }) {
  const isWhite = turn === 'WHITE';

  return (
    <div className="modal-backdrop" role="dialog" aria-modal="true" aria-label="Pawn promotion">
      <div className="modal">
        <h2 className="modal-title">Pawn Promotion</h2>
        <p className="modal-subtitle">Choose a piece to promote your pawn to</p>

        <div className="promotion-grid">
          {OPTIONS.map(opt => (
            <button
              key={opt.value}
              className="promo-btn"
              onClick={() => onChoose(opt.value)}
              aria-label={`Promote to ${opt.label}`}
              id={`promo-${opt.value}`}
            >
              <span className="promo-piece" aria-hidden="true">
                {isWhite ? opt.white : opt.black}
              </span>
              <span className="promo-label">{opt.label}</span>
            </button>
          ))}
        </div>
      </div>
    </div>
  );
}
