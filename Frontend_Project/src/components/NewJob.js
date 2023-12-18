import React, { useState } from 'react';
import './NewModel.css';

export default function NewJob() {
    
    const [customer, setCustomer] = useState("");
    const [start, setStart] = useState("");
    const [days, setDays] = useState("");
    const [location, setLocation] = useState("");
    const [comment, setComment] = useState("");
    const fill= useState("");
    
    let handleSubmit = async e => {
        e.preventDefault();
            fetch('https://localhost:7181/api/Jobs', {
                method: 'POST', // Or PUT
                body: JSON.stringify({
                    customer: customer,
                    startDate: start,
                    days: days,
                    location: location,
                    comments: comment,
                    
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
        <h2>Please add Job information</h2>
        <form onSubmit={handleSubmit}>
            <label>
                <p>Customer</p>
                <input type="text"onChange={e => setCustomer(e.target.value)} />
            </label>
            <label>
                <p>Start Date</p>
                <input type="date" onChange={e => setStart(e.target.value)} />
            </label>
            <label>
                <p>Number of days</p>
                <input type="text" onChange={e => setDays(e.target.value)}/>
            </label>
            <label>
                <p>Location</p>
                <input type="text"onChange={e => setLocation(e.target.value)} />
            </label>
            <label>
                <p>Comment</p>
                <input type="text"onChange={e => setComment(e.target.value)} />
            </label>
            <div>
                <button type="submit">Submit</button>
            </div>
        </form>
    </div>
  );

}
