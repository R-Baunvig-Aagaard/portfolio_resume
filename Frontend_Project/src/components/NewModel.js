import React, { useState } from 'react';
import './NewModel.css';

export default function NewModel() {
    
    const [fName, setFName] = useState("");
    const [lName, setLName] = useState("");
    const [email, setEmail] = useState("");
    const [phone, setPhone] = useState("");
    const [fill , setFill] = useState("");
    
    let handleSubmit = async e => {
        e.preventDefault();
            fetch('https://localhost:7181/api/Models', {
                method: 'POST', // Or PUT
                body: JSON.stringify({
                    firstName: fName,
                    lastName: lName,
                    email: email,
                    phoneNo: phone,
                    addresLine1: fill,
                    addresLine2: fill,
                    zip: fill,
                    city: fill,
                    country: fill,
                    birthDate: "2000-05-05",
                    nationality: fill,
                    height: 5,
                    shoeSize: 20,
                    hairColor: fill,
                    eyeColor: fill,
                    comments: fill,
                    password: fill
                }), // assumes your data is in a
                // form object on your instance.
                credentials: 'include',
                headers: {
                'Authorization': 'Bearer ' + localStorage.getItem("token"),
                'Content-Type': 'application/json'
                }
        }).then(responseJson => {
        this.response = responseJson;
        })
        //.catch(error => alert('Something bad happened: ' + error));
        }

    return(
    <div className="newModel-wrapper ">
        <h2>Please add Model information</h2>
        <form onSubmit={handleSubmit}>
            <label>
                <p>Firstname</p>
                <input type="text"onChange={e => setFName(e.target.value)} />
            </label>
            <label>
                <p>Lastname</p>
                <input type="text" onChange={e => setLName(e.target.value)} />
            </label>
            <label>
                <p>Email</p>
                <input type="email" onChange={e => setEmail(e.target.value)}/>
            </label>
            <label>
                <p>Phone Number</p>
                <input type="text"onChange={e => setPhone(e.target.value)} />
            </label>
            <div>
                <button type="submit">Submit</button>
            </div>
        </form>
    </div>
  );

}
