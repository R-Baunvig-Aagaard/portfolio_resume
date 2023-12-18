import React, { useState } from 'react';
import './NewModel.css';

export default function NewExpense() {
    
    const [modelId, setModelId] = useState("");
    const [jobId, setJobId] = useState("");
    const [date, setDate] = useState("");
    const [text, setText] = useState("");
    const [amount, setAmount] = useState("");
    
    
    let handleSubmit = async e => {
        e.preventDefault();
            fetch('https://localhost:7181/api/Expenses', {
                method: 'POST', // Or PUT
                body: JSON.stringify({
                    modelId: modelId,
                    jobId: jobId,
                    date: date,
                    text: text,
                    amount: amount

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
        <h2>Please add Expense information</h2>
        <form onSubmit={handleSubmit}>
            <label>
                <p>Model Id</p>
                <input type="text"onChange={e => setModelId(e.target.value)} />
            </label>
            <label>
                <p>Job ID</p>
                <input type="text" onChange={e => setJobId(e.target.value)} />
            </label>
            <label>
                <p>Date</p>
                <input type="date" onChange={e => setDate(e.target.value)}/>
            </label>
            <label>
                <p>Comment</p>
                <input type="text"onChange={e => setText(e.target.value)} />
            </label>
            <label>
                <p>Amount</p>
                <input type="text"onChange={e => setAmount(e.target.value)} />
            </label>
            <div>
                <button type="submit">Submit</button>
            </div>
        </form>
    </div>
  );

}
