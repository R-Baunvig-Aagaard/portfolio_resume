// From SOLO22

import './App.css';
import React, { useState } from 'react';
import {
  BrowserRouter as Router,
  Routes,
  Route,
} from "react-router-dom";
import jwt_decode from 'jwt-decode';

import { PageLayout } from './components/PageLayout';
import  NewModel from './components/NewModel';
import Login from './components/Login';
import ModelList from './components/ModelList';
import NewManager from './components/NewManager';
import NewJob from './components/NewJob';
import AddModelJob from './components/AddModelJob';
import RemoveModelJob from './components/RemoveModelJob';
import JobList from './components/JobList';
import NewExpense from './components/NewExpense';


function App() {
  const [token, setToken]  = useState();
  if(!token) {
    return <Login setToken={setToken} />
  }

  var decoded = jwt_decode(localStorage.getItem("token"));
  console.log(decoded);
  if(decoded.ModelId === "-1")
  {
    return (
      <Router>
        <div className="App">
        <h1> Application </h1>
          <Routes>
            <Route path="/" element={<PageLayout />} >
            <Route path="/modellist" element={<ModelList />} />
            <Route path="/joblist" element={<JobList />} />
            <Route path="/newmodel" element={<NewModel />} />
            <Route path="/newjob" element={<NewJob />} />
            <Route path="/addmodeljob" element={<AddModelJob />} />
            <Route path="/removemodeljob" element={<RemoveModelJob />} />
            <Route path="/newmanager" element={<NewManager />} />  
            <Route path="/newexpense" element={<NewExpense />} />  
            </Route>
          </Routes>
        </div>
      </Router>
    )
  }
  if(decoded.ModelId > "0") {
    return (
      <Router>
        <div className="App">
        <h1> Application </h1>
          <Routes>
            <Route path="/" element={<PageLayout />} >
            <Route path="/joblist" element={<JobList />} />
            <Route path="/newexpense" element={<NewExpense />} />  
            </Route>
          </Routes>
        </div>
      </Router>
    )
  }
} 



export default App;
