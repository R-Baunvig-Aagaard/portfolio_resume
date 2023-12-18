// From SOLO22
import { NavLink } from "react-router-dom";

export function Navbar() {
  return (
    <nav >
    <NavLink to="/" >
        Home
      </NavLink>
      <br></br>
      <NavLink to="/modellist" >
        Model List
      </NavLink>
      <br></br>
      <NavLink to="/joblist" >
        Job List
      </NavLink>
      <br></br>
      <NavLink to="/newmodel" >
        Add Model
      </NavLink>
      <br></br>
      <NavLink to="/newJob" >
        Add Job
      </NavLink>
      <br></br>
      <NavLink to="/newmanager" >
        Add Manager
      </NavLink>
      <br></br>
      <NavLink to="/addmodeljob" >
        Add Model to Job
      </NavLink>
      <br></br>
      <NavLink to="/removemodeljob" >
        Remove Model from Job
      </NavLink>
      <br></br>
      <NavLink to="/newexpense" >
        Add new expense
      </NavLink>
      
    </nav>
  );
}