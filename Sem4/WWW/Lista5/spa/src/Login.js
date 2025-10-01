import React, { useState } from 'react';
import './App.css';

function Login({ onLogin, error }) {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [localError, setLocalError] = useState(null);

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const response = await fetch('http://localhost:3001/api/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username, password }),
      });
      if (!response.ok) throw new Error('Błąd logowania');
      const data = await response.json();
      localStorage.setItem('token', data.token);
      setLocalError(null);
      onLogin(username, password); // Powiadom App.js o sukcesie
    } catch (err) {
      setLocalError(err.message);
    }
  };

  return (
    <div className="App">
      <h1>Zaloguj się</h1>
      {(error || localError) && <div>Błąd: {error || localError}</div>}
      <form onSubmit={handleSubmit}>
        <input
          type="text"
          placeholder="Login"
          value={username}
          onChange={e => setUsername(e.target.value)}
        />
        <input
          type="password"
          placeholder="Hasło"
          value={password}
          onChange={e => setPassword(e.target.value)}
        />
        <button type="submit">Zaloguj</button>
      </form>
    </div>
  );
}

export default Login;