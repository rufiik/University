import React, { useEffect, useState } from 'react';

function GamesList({ setError }) {
  const [games, setGames] = useState([]);
  const [title, setTitle] = useState('');
  const [description, setDescription] = useState('');
  const [editId, setEditId] = useState(null);
  const [editTitle, setEditTitle] = useState('');
  const [editDescription, setEditDescription] = useState('');
  const [userRole, setUserRole] = useState('');

useEffect(() => {
  const token = localStorage.getItem('token');
  if (token) {
    try {
      const payload = JSON.parse(atob(token.split('.')[1]));
      setUserRole(payload.role);
    } catch {
      setUserRole('');
    }
  }
}, []);
  // Pobieranie gier
  const fetchGames = async () => {
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/games', {
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd pobierania gier');
      const data = await response.json();
      setGames(data);
    } catch (err) {
      setError && setError(err.message);
    }
  };

  useEffect(() => {
    fetchGames();
    // eslint-disable-next-line
  }, []);

  // Dodawanie gry
  const handleAddGame = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/games', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ title, description }),
      });
      if (!response.ok) throw new Error('Błąd dodawania gry');
      setTitle('');
      setDescription('');
      fetchGames();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Usuwanie gry
  const handleDeleteGame = async (id) => {
    if (!window.confirm('Na pewno usunąć tę grę?')) return;
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/games/${id}`, {
        method: 'DELETE',
        headers: { 'Authorization': `Bearer ${token}` }
      });
      if (!response.ok) throw new Error('Błąd usuwania gry');
      fetchGames();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  // Rozpocznij edycję gry
  const startEdit = (game) => {
    setEditId(game.id || game._id);
    setEditTitle(game.title);
    setEditDescription(game.description);
  };

  // Zapisz edytowaną grę
  const handleEditGame = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch(`http://localhost:3001/api/games/${editId}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ title: editTitle, description: editDescription }),
      });
      if (!response.ok) throw new Error('Błąd edycji gry');
      setEditId(null);
      setEditTitle('');
      setEditDescription('');
      fetchGames();
    } catch (err) {
      setError && setError(err.message);
    }
  };

  return (
    <div className="main-container">
      <h2>Lista gier</h2>
      <form onSubmit={handleAddGame}>
        <input
          type="text"
          placeholder="Tytuł"
          value={title}
          onChange={e => setTitle(e.target.value)}
          required
        />
        <input
          type="text"
          placeholder="Opis"
          value={description}
          onChange={e => setDescription(e.target.value)}
          required
        />
        <button type="submit">Dodaj grę</button>
      </form>
      <ul>
        {games.map(game => (
          <li key={game.id || game._id}>
            {editId === (game.id || game._id) ? (
              <form onSubmit={handleEditGame}>
                <input
                  type="text"
                  value={editTitle}
                  onChange={e => setEditTitle(e.target.value)}
                  required
                />
                <input
                  type="text"
                  value={editDescription}
                  onChange={e => setEditDescription(e.target.value)}
                  required
                />
                <button type="submit">Zapisz</button>
                <button type="button" onClick={() => setEditId(null)}>Anuluj</button>
              </form>
            ) : (
              <>
                <b>{game.title}</b> - {game.description}
                    {userRole === 'admin' && (
                <button onClick={() => startEdit(game)} >Edytuj</button>   )} 
                {userRole === 'admin' && (
                    <button onClick={() => handleDeleteGame(game.id || game._id)} >Usuń</button>
                )}
              </>
            )}
          </li>
        ))}
      </ul>
    </div>
  );
}

export default GamesList;