import torch
import torch.nn as nn
import torch.optim as optim
import random
import numpy as np
import os

from model import PongDQN
from memory import ReplayBuffer

class PongAgent:
    def __init__(self, input_size=5):
        # Hyperparameter
        self.gamma = 0.99                
        self.epsilon = 1.0               
        self.epsilon_min = 0.01          
        self.epsilon_decay = 0.99999      
        self.batch_size = 64             
        self.learning_rate = 0.001       

        self.model = PongDQN(input_size=input_size, output_size=3)
        self.target_model = PongDQN(input_size=input_size, output_size=3)
        
        self.target_model.load_state_dict(self.model.state_dict())
        self.target_model.eval() 
        
        self.optimizer = optim.Adam(self.model.parameters(), lr=self.learning_rate)
        self.criterion = nn.MSELoss()    
        self.memory = ReplayBuffer(capacity=100_000)

    def get_action(self, state):
        if random.random() < self.epsilon:
            return random.randint(0, 2)
            
        state_tensor = torch.FloatTensor(state).unsqueeze(0) 
        
        with torch.no_grad(): 
            q_values = self.model(state_tensor)
            
        return torch.argmax(q_values).item()
    
    def update_target_network(self):
        self.target_model.load_state_dict(self.model.state_dict())

    def train_step(self):
        if len(self.memory) < self.batch_size:
            return None

        states, actions, rewards, next_states, dones = self.memory.sample(self.batch_size)

        states_tensor = torch.FloatTensor(states)
        actions_tensor = torch.LongTensor(actions).unsqueeze(1)
        rewards_tensor = torch.FloatTensor(rewards).unsqueeze(1)
        next_states_tensor = torch.FloatTensor(next_states)
        dones_tensor = torch.FloatTensor(dones).unsqueeze(1) 

        curr_Q = self.model(states_tensor).gather(1, actions_tensor)

        with torch.no_grad():
            next_Q = self.target_model(next_states_tensor).max(1)[0].unsqueeze(1)
            target_Q = rewards_tensor + (self.gamma * next_Q * (1 - dones_tensor))

        loss = self.criterion(curr_Q, target_Q)
        
        self.optimizer.zero_grad() 
        loss.backward()            
        self.optimizer.step()      

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

        return loss.item()

    def save_model(self, filename="pong_brain.pth"):
        torch.save(self.model.state_dict(), filename)
        print(f"\n[Speichern] Gewichte erfolgreich unter '{filename}' gesichert!")

    def load_model(self, filename="pong_brain.pth"):
        if os.path.exists(filename):
            self.model.load_state_dict(torch.load(filename))
            self.model.eval() 
            self.target_model.load_state_dict(self.model.state_dict())
            print(f"\n[Laden] Gewichte '{filename}' erfolgreich geladen!")
        else:
            print(f"\n[Fehler] Datei '{filename}' nicht gefunden.")