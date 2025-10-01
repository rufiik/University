import React, { useState } from 'react';

function UserProfile({ setError }) {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [message, setMessage] = useState('');

  const handleUpdate = async (e) => {
    e.preventDefault();
    try {
      const token = localStorage.getItem('token');
      const response = await fetch('http://localhost:3001/api/users/me', {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${token}`
        },
        body: JSON.stringify({ username, password }),
      });
      if (!response.ok) throw new Error('Błąd aktualizacji profilu');
      setMessage('Profil zaktualizowany!');
      setError && setError(null);
    } catch (err) {
      setMessage('');
      setError && setError(err.message);
    }
  };

  return (
    <div>
      <h2>Profil użytkownika </h2>
      <form onSubmit={handleUpdate}>
        <input
          type="text"
          placeholder="Nowa nazwa użytkownika"
          value={username}
          onChange={e => setUsername(e.target.value)}
        />
        <input
          type="password"
          placeholder="Nowe hasło"
          value={password}
          onChange={e => setPassword(e.target.value)}
        />
        <button type="submit">Zmień dane</button>
      </form>
      {message && <div style={{color: 'green'}}>{message}</div>}
    </div>
  );
}

export default UserProfile;