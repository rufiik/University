import React, {useState } from 'react';
import { BrowserRouter as Router, Routes, Route, Link, Navigate } from 'react-router-dom';
import './App.css';
import Login from './Login';
import Games from './views/Games';
import Reviews from './views/Reviews';
import Users from './views/Users';
import UserProfile from './views/UserProfile';



function App() {
  const [error, setError] = useState(null);
  const [isLogged, setIsLogged] = useState(!!localStorage.getItem('token'));

  const handleLogin = async (username, password) => {
    try {
      const response = await fetch('http://localhost:3001/api/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username, password }),
      });
      if (!response.ok) throw new Error('Błąd logowania');
      const data = await response.json();
      localStorage.setItem('token', data.token);
      setIsLogged(true);
      setError(null);
    } catch (err) {
      setError(err.message);
    }
  };
  const handleLogout = () => {
    localStorage.removeItem('token');
    setIsLogged(false);
  };
const getUserRole = () => {
  const token = localStorage.getItem('token');
  if (!token) return null;
  try {
    const payload = JSON.parse(atob(token.split('.')[1]));
    return payload.role;
  } catch {
    return null;
  }
};
const userRole = getUserRole();
  if (!isLogged) {
    return <Login onLogin={handleLogin} error={error} />;
  }

  return (
    <Router>
      <nav>
      <Link to="/">Strona Główna</Link> |{' '}
      <Link to="/games">Gry</Link> |{' '}
      <Link to="/reviews">Recenzje</Link> |{' '}
      {userRole === 'admin' && (
        <>
          <Link to="/users">Użytkownicy</Link> |{' '}
        </>
      )}
      <Link to="/profile">Profil</Link> |{' '}
      <button onClick={handleLogout}>Wyloguj</button>
      </nav>
      <Routes>
        <Route path="/" element={
          <div className="main-container" style={{textAlign: "center"}}>
            <h1>Witaj w aplikacji!</h1>
            <p>Zarządzaj grami, recenzjami i użytkownikami.<br /></p>
          </div>
        } />
        <Route path="/games" element={<Games setError={setError} />} />
        <Route path="/reviews" element={<Reviews setError={setError} />} />
        <Route path="/users" element={userRole === 'admin' ? <Users setError={setError} /> : <Navigate to="/" />}/>
        <Route path="/profile" element={<UserProfile setError={setError} />} />
        <Route path="*" element={<Navigate to="/" />} />
      </Routes>
      {error && <div style={{color: 'red'}}>Błąd: {error}</div>}
    </Router>
  );
}

export default App;