import React, { useState, useEffect } from 'react';
import Link from 'next/link';
import { useRouter } from 'next/router';
import AuthForm from './AuthForm';

function Home() {
  const [showAuthModal, setShowAuthModal] = useState(true);
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [username, setUsername] = useState('');
  const [role, setRole] = useState('');
  const router = useRouter();

  useEffect(() => {
    const loggedIn = localStorage.getItem('isLoggedIn');
    const storedUsername = localStorage.getItem('username');
    const storedRole = localStorage.getItem('role');
    if (loggedIn && storedUsername && storedRole) {
      setIsLoggedIn(true);
      setUsername(storedUsername);
      setRole(storedRole);
      setShowAuthModal(false);
    }
  }, []);

  const handleLogin = async (username, password) => {
    const response = await fetch('http://localhost:8080/api/auth/login', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ username, password }),
    });
    const data = await response.json();
    if (data.message === 'Login successful') {
      setIsLoggedIn(true);
      setUsername(username);
      setRole(data.role); // Ustawienie roli po zalogowaniu
      setShowAuthModal(false);
      localStorage.setItem('isLoggedIn', 'true');
      localStorage.setItem('username', username);
      localStorage.setItem('role', data.role);
    } else {
      alert('Login failed');
    }
  };

  const handleRegister = async (username, password, email) => {
    const response = await fetch('http://localhost:8080/api/auth/register', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ username, password, email }),
    });
    const data = await response.json();
    if (data.message === 'User registered successfully') {
      alert('Registration successful');
    } else {
      alert('Registration failed');
    }
  };

  const handleLogout = () => {
    setIsLoggedIn(false);
    setUsername('');
    setRole(''); // Resetowanie roli po wylogowaniu
    localStorage.removeItem('isLoggedIn');
    localStorage.removeItem('username');
    localStorage.removeItem('role');
    setShowAuthModal(true);
  };

  const navigateToUsers = () => {
    if (role !== 'USER') {
      router.push('/Users');
    } else {
      alert('Nie masz uprawnień do przeglądania tej strony.');
    }
  };

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-200 p-4 space-y-4">
      {showAuthModal && (
        <div className="fixed inset-0 flex items-center justify-center bg-black bg-opacity-50">
          <div className="bg-white p-8 rounded shadow-md w-full max-w-md">
            <AuthForm onLogin={handleLogin} onRegister={handleRegister} />
          </div>
        </div>
      )}
      {!showAuthModal && (
        <>
          <div className="flex justify-center items-center w-full max-w-4xl">
          {role === 'ADMIN' ? (
            <h1 className="text-4xl font-bold text-blue-600 mb-6">
              Witamy w systemie zarządzania biblioteką!
            </h1>
          ) : null}
            
          {role === 'USER' ? (
            <h1 className="text-4xl font-bold text-blue-600 mb-6">
              Witamy na stronie Naszej Biblioteki!
            </h1>
          ) : null}

            <div className="flex items-center space-x-4 absolute top-4 right-4">
              <span className="text-lg text-gray-700">Zalogowany jako: {username}</span>
              <button
                onClick={handleLogout}
                className="bg-red-500 text-white px-4 py-2 rounded hover:bg-red-600 transition">
                Wyloguj się
              </button>
            </div>
          </div>
          <p className="text-lg text-gray-700 mb-6">
            Wybierz jedną z poniższych opcji, aby zarządzać danymi:
          </p>
          <nav className="space-y-8 w-full flex flex-col items-center">
            <Link href="/Books">
              <span className="bg-blue-500 text-white px-6 py-3 rounded hover:bg-blue-600 transition cursor-pointer">
                Książki
              </span>
            </Link>
            {role !== 'USER' && (
              <span
                onClick={navigateToUsers}
                className="bg-green-500 text-white px-6 py-3 rounded hover:bg-green-600 transition cursor-pointer"
              >
                Użytkownicy
              </span>
            )}
            <Link href="/Reservations">
              <span className="bg-yellow-500 text-white px-6 py-3 rounded hover:bg-yellow-600 transition cursor-pointer">
                Rezerwacje
              </span>
            </Link>
            <Link href="/Penalty">
              <span className="bg-red-500 text-white px-6 py-3 rounded hover:bg-red-600 transition cursor-pointer">
                Kary
              </span>
            </Link>
          </nav>
        </>
      )}
    </div>
  );
}

export default Home;