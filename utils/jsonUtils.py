from threading import Thread
import json
from queue import Queue
import time


stat_path = './data/stat.json'   # 状态文件路径
class JsonDB:
    def __init__(self) -> None:
        self.update_queue = Queue()
        self.t_update()
    
    
    def get_document(self):
        with open(stat_path, 'r') as f:
            data = f.read()
            if data:
                doc = json.loads(data)
                return doc
            else:
                return {}
    
    
    def update_document(self, document):
        self.update_queue.put(document)
    
    
    def _update(self):
        while True:
            if not self.update_queue.empty():
                doc = self.update_queue.get()
                with open(stat_path, 'w+') as f:
                    f.write(json.dumps(doc))
            else:
                time.sleep(0.5)
    
                
    def t_update(self):
        Thread(target=self._update, daemon=True).start()
                