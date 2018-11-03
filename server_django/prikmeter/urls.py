from django.urls import path
import django.contrib.auth.views as auth_views

from . import views

app_name = 'prikmeter'
urlpatterns = [
    path('', views.index, name='index'),
    path('login/', views.login, name='login'),
    path('logout/', views.logout, name='logout'),
]
