export default function LandingPage({ onPlay }) {
  return (
    <main className="landing" aria-labelledby="landing-title">
      <nav className="landing-nav" aria-label="Main navigation">
        <div className="landing-brand"><span aria-hidden="true">♟</span> Chess<span>.</span></div>
        <a href="https://github.com/sagar-2529/CHESS-ENGINE" target="_blank" rel="noreferrer">
          View source <span aria-hidden="true">↗</span>
        </a>
      </nav>

      <section className="landing-hero">
        <div className="landing-copy">
          <p className="landing-kicker">A C++ chess engine</p>
          <h1 id="landing-title">Think ahead.<br /><em>Play boldly.</em></h1>
          <p className="landing-description">
            A complete two-player chess experience powered by a C++ rules engine.
            Every move is validated on the server.
          </p>
          <button className="landing-play" onClick={onPlay}>
            Play a game <span aria-hidden="true">→</span>
          </button>
          <p className="landing-note">No sign-up required · Standard chess rules</p>
        </div>

        <div className="landing-board" aria-hidden="true">
          <div className="landing-board-glow" />
          <div className="landing-grid">
            {Array.from({ length: 64 }, (_, index) => {
              const row = Math.floor(index / 8);
              const col = index % 8;
              const piece = ({
                '0-0': '♜', '0-1': '♞', '0-2': '♝', '0-3': '♛', '0-4': '♚', '0-5': '♝', '0-6': '♞', '0-7': '♜',
                '1-0': '♟', '1-1': '♟', '1-2': '♟', '1-3': '♟', '1-4': '♟', '1-5': '♟', '1-6': '♟', '1-7': '♟',
                '6-0': '♙', '6-1': '♙', '6-2': '♙', '6-3': '♙', '6-4': '♙', '6-5': '♙', '6-6': '♙', '6-7': '♙',
                '7-0': '♖', '7-1': '♘', '7-2': '♗', '7-3': '♕', '7-4': '♔', '7-5': '♗', '7-6': '♘', '7-7': '♖',
              })[`${row}-${col}`];
              return <span key={index} className={(row + col) % 2 ? 'landing-square dark' : 'landing-square light'}>{piece}</span>;
            })}
          </div>
        </div>
      </section>

      <footer className="landing-footer">
        <span>Built with C++ · Crow · React</span>
        <span>Chess engine project</span>
      </footer>
    </main>
  );
}
